#/bin/bash

outfile=bgm_z.cpp
cat include/bilgamesh_internal.hh *.cc > tmp_file.cpp
sed -i "/pragma/d" tmp_file.cpp
sed -i "/BILGA/d" tmp_file.cpp

grep "include" tmp_file.cpp > tmp_headers
sed -i 's:#include "bilgamesh_internal.hh"::' tmp_headers
cat tmp_headers | sort | uniq > tmp_headers2


grep -v "include" tmp_file.cpp >$outfile
mv $outfile tmp_file.cpp
sed -i 's:    ::g' tmp_file.cpp
sed -i 's:   ::g' tmp_file.cpp
gcc -fpreprocessed -dD -E tmp_file.cpp >$outfile
rm tmp_file.cpp
sed -i '/^#/d' $outfile # Delete inserted line numbers.

mv $outfile tmp_file
cat tmp_headers2 >$outfile
echo "using namespace std;" >> $outfile
tr "\t\n" " " <tmp_file >> $outfile
sed -i 's:_bgm_action:_bgm_a:g' $outfile
sed -i 's:_bgm_board:_bgm_b:g' $outfile
sed -i 's:  ::g' $outfile
sed -i 's:board:brd:g' $outfile
sed -i 's:king:k:g' $outfile
sed -i 's: + :+:g' $outfile
sed -i 's: - :-:g' $outfile
sed -i 's: (:(:g' $outfile
sed -i 's:) :):g' $outfile
sed -i 's: {:{:g' $outfile
sed -i 's:} :}:g' $outfile
sed -i 's:, :,:g' $outfile
sed -i 's:; :;:g' $outfile
sed -i 's:black:blk:g' $outfile
sed -i 's:capture:cptr:g' $outfile
sed -i 's:move:mv:g' $outfile
sed -i 's: = :=:g' $outfile
sed -i 's: -= :-=:g' $outfile
sed -i 's: += :+=:g' $outfile
sed -i 's: == :==:g' $outfile
sed -i 's: << :<<:g' $outfile
sed -i 's/std:://g' $outfile

clang++ -std=c++11 -c $outfile

exit
