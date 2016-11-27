for midi in midis/*.mid
do
    midi_filename="${midi%%.*}"
    midi_filename="${midi_filename##*/}"
    musescore -o "sources/${midi_filename}.xml" "$midi"
done
ls sources/*.xml > masterList.txt
