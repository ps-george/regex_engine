#!/bin/bash

# Compare output of programs directly
echo "========================================"
echo "Cleaning the temporaries and outputs"
make clean
echo "Force building regex engine"
make bin/regex_engine -B

if [[ "$?" -ne 0 ]]; then
    echo "Build failed.";
fi

echo "Force building regex engine reference"
make regex_engine_ref -B
echo "========================================="

chmod u+x bin/regex_engine
chmod u+x regex_engine_ref
PASSED=0
CHECKED=0

for i in test/*.txt; do
  echo "Input file : ${i}"
  exec 6< $i
  read -r regex <&6
     
  BASENAME=$(basename $i .txt);
  diff <(tail --lines=+2 $i | eval ./regex_engine_ref ${regex}) <(tail --lines=+2 $i | eval ./bin/regex_engine ${regex})
  if [ $? -ne 0 ]; then
    echo -e "\nERROR"
  else
    PASSED=$(( ${PASSED}+1 ));
  fi
  CHECKED=$(( ${CHECKED}+1 ));
  exec 6<&-
done

echo ""
echo "Passed ${PASSED} out of ${CHECKED} tests."

if [ "${PASSED}" == "${CHECKED}" ]; then
  exit 0
else
  exit 1
fi
