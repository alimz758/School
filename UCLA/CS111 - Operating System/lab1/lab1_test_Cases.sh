# Create testing files.
cat > a0.txt <<'EOF'
Hello world! CS 111! Knowledge crowns those who seek her.
EOF
cat a0.txt > a1.txt
cat a0.txt > a2.txt
cat a0.txt > a3.txt
cat a0.txt > a4.txt
cat a0.txt > a5.txt
cat a0.txt > a6.txt
cat a0.txt > a7.txt
cat a0.txt > a8.txt

cat > b0.txt <<'EOF'
FEAR IS THE PATH TO THE DARK SIDE...FEAR LEADS TO ANGER...ANGER LEADS TO HATE...HATE LEADS TO SUFFERING.
DO. OR DO NOT. THERE IS NO TRY.
EOF
cp b0.txt b1.txt


echo ""
echo "Test 1"
./simpsh --pipe --command 0 1 1 sleep 1 --creat --profile --rdwr test1io.txt \
>c1out.txt 2>c1err.txt
if [ $? == 0 ] && [ ! -s c1err.txt ] && [ -s c1out.txt ] && [ -e test1io.txt ] \
	&& [ ! -s test1io.txt ] && wc -l < c1out.txt | grep -q "1"
then
	echo "Passed Test 1"
else
	echo "Failed Test 1"
fi

echo ""
echo "Test 2 "
./simpsh --creat --excl --rdwr test2io.txt --command 0 0 0 time -p sleep 1 \
--pipe --wait --rdonly a8.txt --profile --command 3 2 0 cat --command 1 0 0 cat \
>c2out.txt 2>c2err.txt
T2RC=$?
sleep 1
if [ $T2RC == 0 ] && [ -e test2io.txt ] && grep -q "real 1." test2io.txt \
&& grep -q "Knowledge crowns those who seek her" test2io.txt \
&& grep -q "Hello world! CS 111! " test2io.txt \
&& grep -q "exit 0 time -p sleep 1" c2out.txt && ! grep -q "cat" c2out.txt \
&& [ ! -s c20err.txt ]
then
	echo "Passed Test 2"
else
	echo "Failed Test 2"
fi