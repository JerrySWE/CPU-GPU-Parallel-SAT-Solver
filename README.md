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
following basic rules:
¬(not) rules:
| x | ¬x|        
| - | --|        
|true|false|     
|false|true|

∨(or) rules:
| x | y | x∨y |
| - | - | --- |
|true|true|true|
|true|false|true|
|false|true|true|
|false|false|false|

∧(and) rules:
| x | y | x∧y |
| - | - | --- |
|true|true|true|
|true|false|fa;se|
|false|true|false|
|false|false|false|

Parenthesis could be used to change the order of evaluation just like arithmetic expressions. Since the SAT problem is proven to be NP complete, in the worst case scenario, we would need to evaluate all possible assignment of variables to decide the satisfiability of the boolean expression. The number of possible assignments grows exponentially with the amount of variables since every variable have two possible assignments, true or false.

Note that this exhaustive search across an exponential amount of assignments would benefit greatly from parallelism since the evaluation of each assignment is completely independent from each other and each worker could be assigned with a certain segments of the search and evaluate different assignments themselves. Additionally, other approaches to SAT solving, such as those using DPLL logic and conflict-driven solving, offer speedup compared to the naive implementation and could possibly even further improve from a parallel environment.

Since we could use boolean algebra to convert any arbitrary boolean formula into conjunctive normal form,
our program will only solve SAT problems in conjunctive normal form. If we have time, we could write a
converter that will automatically convert any boolean formula into CNF.

A conjunctive normal form is where a boolean formula is only consist of conjunctive of clauses. A clause
is a disjunction of literals. Literals are either a variable or the negation of a variable.

A conjunctive normal form is where a boolean formula is only consist of conjunctive of clauses. A clause
is a disjunction of literals. Literals are either a variable or the negation of a variable.

**x, ¬x**

Similarly, x, ¬x and y are literals and if we connect them together with ∨ to form a disjunction, we have
a clause:

**x ∨ ¬x ∨ y**

To put everything together, if we connect the clauses together with ∧ to form a conjunction of disjunctions,
we have a boolean formula that is in conjunctive normal form:

**(x ∨ ¬x ∨ y) ∧ (z ∨ ¬y ∨ x) ∧ (¬x ∨ ¬z ∨ ¬y)**

Note that the CNF above is satisfiable with the assignment x = true, y = f alse, z = f alse since if we plug
in the assignment into the CNF, we have:

**(x ∨ ¬x ∨ y) ∧ (z ∨ ¬y ∨ x) ∧ (¬x ∨ ¬z ∨ ¬y)**
**⇐⇒(T ∨ ¬T ∨ F) ∧ (F ∨ ¬F ∨ T) ∧ (¬T ∨ ¬F ∨ ¬F)**
**⇐⇒(T ∨ F ∨ F) ∧ (F ∨ T ∨ T) ∧ (F ∨ T ∨ T)**
**⇐⇒T ∧ T ∧ T**
**T**

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
