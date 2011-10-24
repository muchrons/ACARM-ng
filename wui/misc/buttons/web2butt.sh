#!/bin/bash
DEBUG=0
mkdir -p out
OUT_DIR=out

#Button Style
COLOR1=#98a1ad
COLOR2=#0d1f35
TEXT_COLOR=white
TEXT_SHADOW=black


TEXT_SIZE=14
TEXT_FONT='Helvetica'
BORDER_SIZE=2
BORDER_ROUND=10
BORDER_COLOR=$COLOR2
GRADIENT=${COLOR1}-${COLOR2}


BORDER_MARGIN=0$(echo "scale=1; $BORDER_SIZE/2" | bc)
i=1

function button
{
    BUTTON_WIDTH=$2;
    BUTTON_HEIGHT=$3;

    TEXT=`echo $LINE | cut -d';' -f1`;
    FILE=`echo $LINE | cut -d';' -f2`;

    BUTT="$OUT_DIR/${FILE}";

    GRAD=`mktemp`.miff;
    RBORDER=`mktemp`.mvg
    OVERLAY=`mktemp`.miff;
    OVERLAY2=`mktemp`.miff;
    MASK=`mktemp`.miff;

    # Create Gradient
    convert -size $(expr $BUTTON_WIDTH - 2)x$(expr $BUTTON_HEIGHT - 2) gradient:$GRADIENT $GRAD

    # Create the round corners and store as Magick Vector Graphics
    convert $GRAD -border 1 \
        -format "roundrectangle $BORDER_MARGIN,$BORDER_MARGIN %[fx:w-$BORDER_MARGIN-1], %[fx:h-$BORDER_MARGIN-1] $BORDER_ROUND, $BORDER_ROUND" \
        info: > $RBORDER

    # Button Border
    convert $GRAD -border 1 -matte -channel RGBA -threshold -1 \
        -background none -fill none -stroke $BORDER_COLOR -strokewidth $BORDER_SIZE \
        -draw "@$RBORDER"    $OVERLAY

    # Add Text to Overlay
    convert $OVERLAY -font $TEXT_FONT -pointsize $TEXT_SIZE \
        -fill $TEXT_SHADOW -draw "gravity Center text 1,1 '$TEXT'" \
        -fill $TEXT_COLOR -draw "gravity Center text 0,0 '$TEXT'" \
        $OVERLAY2

    # Mask for the Button
    convert $GRAD -border 1 -matte -channel RGBA -threshold -1 \
        -background none -fill white -stroke none -strokewidth 0 \
        -draw "@$RBORDER"    $MASK

    # Join the images
    convert $GRAD -matte -bordercolor none -border 1 \
       $MASK -compose DstIn -composite \
       $OVERLAY2 -compose Over -composite \
       -quality 95 PNG32:${BUTT}D.png

    convert -modulate 150,100,100 ${BUTT}D.png ${BUTT}U.png
}


while read LINE; do
    echo $LINE | cut -d';' -f1;
    button "$LINE" 200 29;
    (( i ++ ))
done < big_buttons.txt

while read LINE; do
    echo $LINE | cut -d';' -f1;
    button "$LINE" 180 24;
    (( i ++ ))
done < small_buttons.txt

while read LINE; do
    echo $LINE | cut -d';' -f1;

    FILE=`mktemp`.png;
    TEXT=`echo $LINE | cut -d';' -f1`;
    # Temp Image to get width of text
    convert -strokewidth 6 -background none -fill $TEXT_COLOR -font $TEXT_FONT -pointsize $TEXT_SIZE \
        label:"$TEXT" \
        $FILE
                        
    BUTTON_WIDTH=`convert $FILE -format '%w'   info:`
    BUTTON_WIDTH=$(expr $BUTTON_WIDTH + 20)

    button "$LINE" $BUTTON_WIDTH 24;
    (( i ++ ))
done < exact_buttons.txt



if [ "$DEBUG" -ne "1" ]; then
    rm -f $RBORDER
    rm -f $GRAD
    rm -f $OVERLAY
    rm -f $OVERLAY2
    rm -f $MASK
fi
