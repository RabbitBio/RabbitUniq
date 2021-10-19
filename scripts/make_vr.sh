files=`cat ./uniq_virual.txt`
for file in ${files[@]}
do
  echo "cating" $file
  cat $file >> ./viral.dedup.fna
done
