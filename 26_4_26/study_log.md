##自我学习总结
日志的实现
时间戳转换字符串格式

用户调用 LOG(...) << "xxx"
        │
        ▼
Logger 类（外观层）
        │
        ▼
LogMessage 类（日志构造层）
        │
        ▼
LogStrategy 策略类（输出层）
        ├── ConsoleLogStrategy → 输出到屏幕
        └── FileLogStrategy   → 输出到文件

不知道如何使用一个策略对象来兼容所有策略的类型，后面发现使用基类天然就能接受所有的这类类型，问AI术语为：基类指针（或引用）多态，配合 工厂模式 或 依赖注入

今天的有点难度了，设计上和代码上：
设计上把握住分层、分类，像开始刷新策略模块 和 使用刷新策略并构建日志信息的模块 和 外层包装使用模块 完全混一起，无从下手，
**精妙代码**LogMessage类用来构建日志信息，并用RAII管理，其中重载<< + RAII管理的析构很巧妙，用户使用时<<重载返回自身引用以便链式输入，然后输入结束时临时LogMessage自动析构执行对应策
略将信息输入对应日志
代码上:类的继承，策略用子类来实现多态，各种没怎么用过的接口使用（filesystem的文件属性相关，ofstream文件流的使用等），还有宏的封装使用（宏函数）


##AI总结
一、今天学习内容总结（日志库设计）
1. 日志库三大模块
模块	功能	关键设计
刷新策略（LogStrategy）	决定日志输出到哪里	纯虚基类，派生出 ConsoleLogStrategy、FileLogStrategy
日志构造（LogMessage）	拼接日志内容，RAII 自动输出	临时对象析构时调用策略的 SyncLog
外观（Logger）	统一接口，管理策略	重载 operator() 返回 LogMessage 临时对象

2. 核心设计模式
模式	应用
策略模式	LogStrategy 接口 + 不同实现
RAII	LogMessage 构造时收集内容，析构时输出
外观模式	Logger 对用户隐藏内部细节
流式接口	operator<< 支持连续拼接

3. 宏封装
cpp
#define LOG(level) logger(level, __FILE__, __LINE__)
自动捕获 __FILE__ 和 __LINE__
用户无需手动传文件名和行号

4. 关键技术点
operator() 重载：让 logger 对象像函数一样调用
临时对象生命周期：LOG(INFO) << "msg" 产生的临时对象在语句结束时析构
std::filesystem：C++17 创建目录（create_directories）
std::ofstream：追加模式打开日志文件