echo "Lab1c Tests"
echo "-------------------------------" 

files="$files err4 err5 err6 err7 err8 out4 out5 out6"
for i in $files
do
	touch $i; chmod ugo+rw $i
done

# this uses pg98.txt. If you don't have it, get it
if [ ! -e pg98.txt.txt ]; then \
	wget https://d1b10bmlvqabco.cloudfront.net/attach/k12d141vub1ha/i1ibgf1mdy91kq/k1va8gvdbhp5/pg98.txt
fi
chmod ugo+r pg98.txt

# Test Case 1: ( cat < pg98.txt | sort | grep French > out4) 2>> err4
echo "---Test 1"
./simpsh --profile --rdonly pg98.txt --pipe --pipe --wronly out4 --wronly err4 --command 0 2 6 cat --command 1 4 6 sort --command 3 5 6 grep French --wait
rm out4; touch out4; chmod ugo+rw out4

# Test Case 2: (echo -e "Capitalizing everything...\n\n" < in2 | cat - pg98.txt | tr a-z A-Z > out5) 2>> err5
echo ""
echo "---Test2"
./simpsh --profile --rdonly pg98.txt --pipe --pipe --creat --trunc --wronly out5 --creat --append --wronly err5 --command 0 2 6 echo -e "Capitalizing everything...\n\n" --command 1 4 6 cat - pg98.txt --command 3 5 6 tr a-z A-Z --wait
rm out5; touch out5; chmod ugo+rw out5

# Test Case 3: grep business < pg98.txt 2> err6 | sort 2> err7 | wc > out6 2>> err8
echo ""
echo "---Test 3"
./simpsh --profile --rdonly pg98.txt --creat --trunc --wronly err6 --pipe --creat --trunc --wronly err7 --pipe --creat --trunc --wronly out6 --creat --append --wronly err8 --command 0 3 1 grep business --command 2 6 4 sort --command 5 7 8 wc --wait

rm -f $files
