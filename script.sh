mkdir project
cd project/
mkdir doc include src
echo "Charlotte Cabouat" > README.md
echo "" > include/func.h
echo "" > src/func.c
echo "" > src/main.c
ls -R > contents.txt
cd ..
cp -r project/ projectV3/
rm -rf project/
tar -c -f pv3.tar projectV3/
