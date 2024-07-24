
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [prepare](#prepare)
  - [1. 请先安装好一下相关库,并确保其在g++的搜索路径下](#1-请先安装好一下相关库并确保其在g的搜索路径下)
  - [2. 编辑自身算子需要的cmake](#2-编辑自身算子需要的cmake)
  - [3.在验证平台中加入算子](#3在验证平台中加入算子)
- [run](#run)
- [debug](#debug)

<!-- /code_chunk_output -->

# prepare

## 1. 请先安装好一下相关库,并确保其在g++的搜索路径下
    a) boost 1.84 
    b) half 2.2

## 2. 编辑自身算子需要的cmake
    a) 在cmake中测试条件可做如下设置，可灵活设置以支持不同的test corners
| 测试条件    | 解释       | 支持值              |
|------------|------------|--------------------|
| Rnd_Mode   | 舍入方式    | 0~3                |
| Corner_Case| 激励类型    | reg zero inf nan   |
| Fp_type    | 计算精度    | fp16 fp32 fp64     |
| Checker    | 检测模式    | assert             |

    b) 请将generate_test函数中的第二项改成待测算子名，保证其与之后添加在验证平台算子表中的名字一致。

## 3.在验证平台中加入算子
    a) 请在tool/model/op中的ref和cmodel目录中分别加入需要测试的算子，可参照\*_mul.cpp和\*_mul.h的写法。 
    b) 请在tool/model中的cmodel.h和ref.h的两张表中加入需要测试的算子。

# run
    1. 在cmake目录下新建build文件夹并cd进入
    2. cmake .. -Dtest_times=500,具体数值请根据需求自行设置
    3. make -j 32 进行编译
    4. 运行ctest
        ctest -R "^test_name$" --verbose
        ctest --show-only
        ctest -R "^test_name_match" -j 32
        ctest . --repeat until-fail:10000 -j 32 -R test_name --output-on-failure

# debug
    1. 在build目录下，输入如下命令：
        gdb ./excutable -x ../tool/c_debug/cmd.gdb
        excutable的选择和cmd.gdb的编写请根据需要debug的test自行测试。