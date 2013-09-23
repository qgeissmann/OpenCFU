SAMPLES="./samples"
REFS="references"
out=$(mktemp)

echo "Computing MD5 sums"
i=1;
while read line; do
    ./opencfu -i $SAMPLES/$line 2>/dev/null | md5sum >> $out
    echo $i;
    i=$[$i+1];
done < "./test/queries"

if (("$#" > "0"))
then
    echo "Saving MD5 sums to $SAMPLES/$REFS"
	mv $out $SAMPLES/$REFS
else
	echo "Comparing MD5 sums to $SAMPLES/$REFS"
	a=$(comm $SAMPLES/$REFS  $out  -2 -3 | wc -l)
	if (("$a" == "0"))
		then
		echo "OK: no differences"
	else
		echo "REFERENCE DIFFERS FROM LOCAL COMPUTATION!!!"
	fi
	rm $out;
fi
