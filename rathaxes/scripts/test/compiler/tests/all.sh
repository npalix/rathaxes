for i in `ls *.cws`; do ./test.sh $i > /dev/null && echo "success for test $i"  || echo "failure for test $i" ; done
