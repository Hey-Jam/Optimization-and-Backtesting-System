# Optimization-and-Backtesting-System 更新日志
4/13/2020 7：56AM
Portfolio头文件， 需要stock_pool头文件

----------------------
4/15/2020 Hongfei
- Account头文件
    + 作为一个子成员在portfolio底下,用于记录交易过程细节
    + 设计为在一次回测开始initialize,一直存在到回测结束,每个策略对应一个account
    + balance决定每次各个股票买多少,cash决定实际交易能完成
    + Account有两种跟新方式
        1. 在回测中每次循环的最小时间单位都跟新,用于记录balance和当前日期的变化
        2. 在每次交易行为发生时(rebalance)跟新,除1外用于跟新交易记录和cash account

- 问题
    1. 现在Asset形式没有确定,一些具体的地方如何传值拿不准
    2. 讨论一下进一步要不要用 typedef std::unordered_map<asset*, int> Position; 来控制具体交易信号.
        - 传入Account这样的Position,每个asset需要改变的股数
        - 重载Position的+/-运算符
