#!/bin/sh

function usage() {
  echo "usage ${0##*/} FILES"
  echo "convert Japan-Post data"
  echo
  echo "  FILE1 ... Japan Post data"
  exit 1
}

if [ $# -lt 1 ]; then
  usage
fi

if [ -f $1 ]; then
  jpdata=$1
else
  usage
fi


cat ${jpdata} | nkf | awk -F'[,]' '{ printf "%s,%s,%s,%s\n",substr($1,1,2), $7,$8,$9 }' | sed -e 's/"//g' | sed -e 's/（.*）//g' | grep -v '以下に掲載がない場合' | uniq
