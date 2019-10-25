touch file1
touch file2
touch file3

./simpsh --rdonly file1 --rdonly file2 --rdonly file3

if [ $? -ne 0 ]; then
  echo "--rdonly failed"
  exit 1
fi

./simpsh --wronly file1 --wronly file2 --wronly file3

if [ $? -ne 0 ]; then
  echo "--wronly failed"
  exit 1
fi

./simpsh --wronly NOTAREALFILE 2> /dev/null

if [ $? -eq 0 ]; then
  echo "Should exit with nonzero status for failure to open write-only file"
  exit 1
fi

./simpsh --rdonly file1 --verbose --rdonly file2 | grep -e "\-\-rdonly b" > /dev/null

if [ $? -ne 0 ]; then
  echo "Should print out commands after the verbose and not before"
  exit 1
fi

rm file1
rm file2
rm file3

echo "Successfully passed all tests!"
exit 0