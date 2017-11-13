
#!/usr/bin/env bash

for file in *.dot ; do
	TARGET_FILE="${file%.*}.png"
	echo "Converting ${file} -> ${TARGET_FILE} ..."
	dot $file -Tpng -o $TARGET_FILE
done

echo "Done"
