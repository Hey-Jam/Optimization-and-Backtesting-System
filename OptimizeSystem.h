#ifndef OPTIMIZESYS_H
#define OPTIMIZESYS_H

#include"Portfolio.h"
#include"Account.hpp"
#include"StockPool.hpp"
#include"asset.h"

#include<math.h>
#include<iostream>
#include<string>
#include<vector>

#include<gsl/gsl_blas.h>
#include<gsl/gsl_statistics_double.h>
#include<gsl/gsl_vector.h>
#include<gsl/gsl_matrix.h>
#include<gsl/gsl_multimin.h>


//   OptimizeSystem class is an abstract class, It has several ture virtual functions that any
// derived Optimize Strategy class should have. It doesn't have any class members, just virtual
// functions.

double Sharp(const gsl_vector * weights, void *params) {

    double *p = (double*) params;

    size_t num = weights->size; // the length of weights vector, same as the number of tickers

    gsl_matrix_view cov = gsl_matrix_view_array(p + num, num, num);

    double Er = gsl_stats_wmean(weights->data, 1, p, 1, num);

    gsl_vector *subsigmas = gsl_vector_calloc(num);
    gsl_blas_dgemv(CblasNoTrans, 1, &cov.matrix, weights, 0, subsigmas);

    double *sigma;
    gsl_blas_ddot(weights, subsigmas, sigma);
    gsl_vector_free(subsigmas);

    return Er / sqrt(*sigma);
}

class OptimizeSystem {
public:
    virtual Portfolio* optimize(vector<string>& tickers, boost::gregorian::date Start, boost::gregorian::date End) = 0;
};

class ClassicMarkowitz : public OptimizeSystem {
private:
    StockPool* stockpool = nullptr;
public:
    ClassicMarkowitz(StockPool* _stockpool): stockpool{_stockpool} {}

    ~ClassicMarkowitz() {
        delete stockpool;
    }

    // getter
    // int getSize() const { return stockpool.getsize();} // BUG1 2020-05-03 20:27 hongfei
    const int getSize() const { return stockpool->getSize();}
    Portfolio* optimize(vector<string>& tickers, boost::gregorian::date Start, boost::gregorian::date End) {
        /* the main function of ClassicMarkowitz class, implement the optimization with Markowitz method */

        gsl_matrix *closeprice = stockpool->getPrice(tickers, Start, End); // this part needed to be completed by Jam, column is the ticker, row is the date
        vector<boost::gregorian::date> DateArray;


        size_t num = closeprice->size2; // check tickers.size() == num?
        size_t date_length = closeprice->size1;

        gsl_matrix_view next_price = gsl_matrix_submatrix(closeprice, 1, 0, date_length - 1, num);
        // suppose closeprice(end,0) is the latest price of the first ticker
        gsl_matrix_view last_price = gsl_matrix_submatrix(closeprice, 0, 0, date_length - 1, num);
        gsl_matrix_div_elements(&next_price.matrix, &last_price.matrix);

        gsl_matrix *closereturn = gsl_matrix_alloc(date_length - 1, num);
        gsl_matrix_memcpy(&next_price.matrix, closereturn);
        gsl_matrix_add_constant(closereturn, -1); // r = p_next/p_last - 1

        // This part is to calculate the covariance matrix and mean return vector
        gsl_matrix *cov = gsl_matrix_alloc(num, num);
        gsl_vector *expectreturn = gsl_vector_alloc(num);

        // BUG2 not fixed 2020-05-03 20:37 报错如下
        for (int i = 0; i < tickers.size(); i++) {
            gsl_vector_view R1 = gsl_matrix_column(closereturn, i);
            gsl_vector_set(expectreturn, i, gsl_stats_mean((&R1.vector)->data, 1, date_length));

            for (int j = 0; j <= i; j++) {
                gsl_vector_view R2 = gsl_matrix_column(closereturn, j);

                double sigma_ij = gsl_stats_covariance((&R1.vector)->data, 1, (&R2.vector)->data, 1, date_length - 1);
                gsl_matrix_set(cov, i, j, sigma_ij);
                if (i != j) gsl_matrix_set(cov, j, i, sigma_ij);
            }
        }

        double *p = new double[num + num * num];
        memcpy(p, expectreturn->data, num);
        memcpy(p + num, cov->data, num * num);



        const gsl_multimin_fminimizer_type *T;
        gsl_multimin_fminimizer *Optimize;
        gsl_multimin_function my_func;

        // double Sharp(const gsl_vector * weights, void *params) {

        //   double *p = (double*) params;

        //   size_t num = weights->size; // the length of weights vector, same as the number of tickers

        //   gsl_matrix_view cov = gsl_matrix_view_array(p + num, num * num);

        //   double Er = gsl_stats_wmean(weights, 1, p, 1, num);

        //   gsl_vector *subsigmas = gsl_vector_calloc(num);
        //   gsl_blas_dgemv(CblasNoTrans, 1, &cov.matrix, weights, 0, subsigmas);
        //   double *sigma;
        //   gsl_blas_ddot(weights, subsigmas, sigma);
        //   gsl_vector_free(subsigmas);

        //   return Er / (*sigma);
        // }

        my_func.n = num;
        my_func.f = &Sharp;
        my_func.params = p;

        T = gsl_multimin_fminimizer_nmsimplex;
        Optimize = gsl_multimin_fminimizer_alloc(T, num);

        // initial point of the optimize process
        gsl_vector* x = gsl_vector_alloc(num);
        gsl_vector_set_all(x, 1.0 / (double)num);

        gsl_vector* step_size = gsl_vector_alloc(num);
        gsl_vector_set_all(x, 0.01);

        gsl_multimin_fminimizer_set(Optimize, &my_func, x, step_size);


        size_t iter = 0;
        int status;

        do {
            iter++;
            status = gsl_multimin_fminimizer_iterate(Optimize);

            if (status) break;

            status = gsl_multimin_test_size(gsl_multimin_fminimizer_size(Optimize), 1e-4);

            if (status == GSL_SUCCESS)
                std::cout << "Markowitz Portfolio found" << endl ;
        }

        while (status == GSL_CONTINUE && iter < 100);

        // the answer saved in the Optimize->x, which is a gsl_vector

        Portfolio *OptimizedPortfolio = new Portfolio(*(DateArray.end() - 1), tickers, Optimize->x);
        // in the final part of the optimize function, you should free memory
        gsl_multimin_fminimizer_free(Optimize);
        gsl_vector_free(x);
        gsl_matrix_free(closereturn);
        gsl_matrix_free(cov);
        gsl_vector_free(expectreturn);
        delete[] p;

        return OptimizedPortfolio;

    }

};

#endif

