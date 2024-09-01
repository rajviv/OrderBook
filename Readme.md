# Assumptions
* Trade and Cancel requests are treated as similar, since in general Trade happens between order on both sides. Now since we are only given one orderId, I presumed that we just simply have to remove that order.
* 

# About the files
1. `include` folder contains header files.
2. `src` folder contains the c++ files.
    * `utils` files are for necessary structs to hold the packet data.
    * `decoder` files holds code for **Decoder** class for converting and one by one reading packets from the `.dat` files.
    * `orderbook` files holds code for **OrderBook** class contains the orderbook.
    * `encoder` files holds code for **Encoder** class which is used to print the data to `.dat`file.
3. These all get linked by using make command.
4. `output` folder gets created when we run the orderBook binary on the .dat files.
5. `tests` folder is where I made 2 tests in folders `test1` and `test2`.
6. `tmp` is just the data files provided in the question zip qbdata.tar.gz
7. `responsereader` is the binary provided to read output dat file.
8. `runTests.sh` is the shell script, if run using:
    ```
    $ ./runTests.sh
    ```
    It will run the tests in the `tests` folder. And will compare the output with already put output in the folders.
9. `testGenerator.cpp` is the file which on compiling with aditional files, convert a text file in a folder to .dat binary file.

# How to Run
1. First run the following command to get a binary (orderBook).

    ```
    $ make
    ```

2. Next, to run the orderBook program

    ```
    $ ./orderBook <path to folder containing data files> [optional | nam of output file]
    ```
    eg.
    ```
    $ ./orderBook tmp/thdata
    $ ./orderBook tmp/thdata output1.dat
    $ ./orderBook tests/test1 output_test1.dat
    ```


# Code Explaination
Most of the code is self explanatory. Comments are mentioned to explain code sections.
Assert statements have been used.