#!/usr/bin/env bash



path=$1;

pathFwd=$2;

num=$3;



file="$(< $pathFwd)"



if [[ ! "$file" =~ ^([[:space:]]*[0-9]+[[:space:]]*>[[:space:]]*[0-9]+[[:space:]]*)*$ ]]; then

    echo "File is incorrect."

    exit 1

fi



input_raw="$(< $pathFwd)";



input="NEW A $input_raw ? $num";



echo $input | ./$path >>reverse.out;



arg="NEW A $input_raw";



while read l; do

    arg="$arg $l ?";

done <reverse.out



echo $arg | ./$path >>get.out;



getV=$(< get.out)

reverseV=$(< reverse.out)



arr_get=($getV)

arr_rev=($reverseV)



for (( i=0; i < ${#arr_get[@]}; i++ ))

do

    if [ "${arr_get[i]}" = "$num" ]; then

        echo "${arr_rev[i]}"

    fi

done



rm -f reverse.out

rm -f get.out