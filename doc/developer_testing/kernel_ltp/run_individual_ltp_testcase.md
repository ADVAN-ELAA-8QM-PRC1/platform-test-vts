# How to run individual LTP test case
## 1. Compile VTS and LTP source code
'make vts -j12'

## 2. Start VTS-TradeFed
'vts-tradefed'

## 3. Run individual LTP test from VTS-TradeFed console
'> run vts -m KernelLtpTest -t <testname1,testname2,...>'

Test names in filter can be any of the following formats:
<testsuite-testname>, <testsuite-testname_Nbit>, <testname>

It is recommended to include test suite, i.e., the first two formats above.

Example:

'> run vts -m KernelLtpTest -t syscalls-accept01,mm-mmapstress05'

Test cases specified in this filter will be run regardless whether they are disabled in configuration, unless test case name is listed in definitions or executables were not built.