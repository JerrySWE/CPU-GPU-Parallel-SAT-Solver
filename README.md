# **GPU Accelerated SAT Solver**

## **Team Member**
* Jiarui Wang (jiaruiwa)
* Tejas Badgujar (tbadguja)

[418 Project Link](https://github.com/JerryCMU/CUDA-Accelerated-SAT-Solver)

[418 Project Proposal](https://github.com/JerryCMU/CPU-GPU-Parallel-SAT-Solver/blob/main/Project_Proposal.pdf)

## **Summary**
We will implement a parallel boolean satisfiability problem solver. After implementing the solver with
thread-level parallelism on CPU and CUDA parallelism on GPU, we will run our program on the GHC
machine (or another machine similar in specs) to measure and compare the performance and speedup of
our algorithms.

## **Background**
As the first problem proven to be NP complete, SAT is undoubtedly one of the most iconic and fundamental
computation problem in computer science. It tightly connects to many problems such as type checking,
automated theorem proving, automated circuit verifying and many other fields.

Boolean satisfiability problem can be summarized into the follow sentence: given a boolean formula, is
there an assignment of variables that will evaluate the boolean formula to true.

To add more detail, a boolean formula is composed of some very basic elements. There are three logical
operators ¬(not), ∨(or) and ∧(and) that connect variables together. These logical operator follows the
following basic rules
| x | ¬x|        |x | y| x∨y |
| - | --|        |- | -| ---|
|true|false|     |true|true|true|
|false|true|     |true|false|true|
                 |false|true|true|
                 |false|false|false|

## **The Challenge**


## **Resources**


## **Goals and Deliverables**
* 90%
  * Place Holder
* 100%
  * Place Holder
* 125%
  * Place Holder


## **Platform Choice**

## **Input File Format**

## **MILESTONE**


## **Schedule**
| Week | Items |
| ---- | ----- |
| Week 1 (11/7-11/13)  | Finish writing project proposal and setting up development environment |
| Week 2 (11/14-11/20) | Finish writing the sequential version of SAT Solver that will run on CPU. Parallelize the SAT solver on CPU with thread-level parallelism. |
| Week 3 (11/21-11/27) | Start writing the CUDA implementation of the SAT Solver. |
| Week 4 (11/28-12/4)  | Finish implementing CUDA version of the SAT Solver. |
| Week 5 (12/5-12/11)  | Finalize the code. Measure performance and write up final report. |
| Week 6 (12/12-12/18) | Final Presentation |
