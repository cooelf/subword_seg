
Code accompanying the paper **Effective Subword Segmentation for Text Comprehension**

### Requirement

The only requirement is g++. We have tested on Windows and Linux. 

### Usage 

Make sure the input files are encoded as **UTF-8**.

This tool provides BPE-AV, BPE-DLG, BPE-FRQ to segment words into subwords. 

g++ -o gbpe gbpe.cpp

./gbpe -bpe inputfile [input mode][segmentation] [merging times] output.vocab 

**input mode**:

* 0: plain text
* 1: word vocab with ranked order (auto-generated vocab.txt)

**segmentation**:

* 10: BPE-FRQ
* 11: BPE-AV
* 12: BPE-DLG 

For example, ./gbpe -bpe sample_corpus.txt 0 11 10000 sample.vocob means the input corpus is plain text, using BPE-AV merging 10000 times. 

The code will give 3 outputs. 

* **output.vocab**: the segmented word vocabulary, a segmented word each line, like "com@@ pe@@ ti@@ tion".  @@ is the segmentation symbol.

* **vocab.txt**: word vocab with ranked order, for next quicker segmentation as the inputfile with input model = 1.

* **vocab.substring.txt**: subword vocab (subword with goodness score each line), this file could be used for other segmentations, like MM and Vitebi.

We also provide the segmentation for a new input corpus, based on generated substring dictionary. For example, if you want to segment a new file based on the pre-trained subsword dictionary, this would be useful.

./gbpe -bpeseg [input file] vocab.substring.txt [output file]

To use this function, your output file name should begin with M or N. 

* M: without segmentation symbol "@@ "

* N: with the symbol.

For example:

./gbpe -bpeseg sample_corpus.txt  vocab.substring.txt  Moutput-file.txt

means segmentation without "@@ "(each subword segmented by blank)

./gbpe -bpeseg sample_corpus.txt vocab.substring.txt Noutput-file.txt

means segmentation with "@@ "

### What's more

A relevant usage for downsteam models can be referred to https://github.com/cooelf/subMrc 