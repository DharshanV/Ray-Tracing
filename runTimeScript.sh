#!/bin/bash
start_time="$(date -u +%s%3N)"
echo -e "./renderMain 500 500 0 renderedImage.ppm | \c"
./renderMain 500 500 0 renderedImage.ppm
end_time="$(date -u +%s%3N)"
elapsed="$(($end_time-$start_time))"
inSecs=`bc <<< "scale=2; $elapsed / 1000"`
echo -e "Render time: $inSecs secs"
rm renderedImage.ppm

start_time="$(date -u +%s%3N)"
echo -e "./renderMain 500 500 1 renderedImage.ppm | \c"
./renderMain 500 500 1 renderedImage.ppm
end_time="$(date -u +%s%3N)"
elapsed="$(($end_time-$start_time))"
inSecs=`bc <<< "scale=2; $elapsed / 1000"`
echo -e "Render time: $inSecs secs"
rm renderedImage.ppm

start_time="$(date -u +%s%3N)"
echo -e "./renderMain 500 500 2 renderedImage.ppm | \c"
./renderMain 500 500 2 renderedImage.ppm
end_time="$(date -u +%s%3N)"
elapsed="$(($end_time-$start_time))"
inSecs=`bc <<< "scale=2; $elapsed / 1000"`
echo -e "Render time: $inSecs secs"
rm renderedImage.ppm

start_time="$(date -u +%s%3N)"
echo -e "./renderMain 1600 900 0 renderedImage.ppm | \c"
./renderMain 1600 900 0 renderedImage.ppm
end_time="$(date -u +%s%3N)"
elapsed="$(($end_time-$start_time))"
inSecs=`bc <<< "scale=2; $elapsed / 1000"`
echo -e "Render time: $inSecs secs"
rm renderedImage.ppm

start_time="$(date -u +%s%3N)"
echo -e "./renderMain 1600 900 1 renderedImage.ppm | \c"
./renderMain 1600 900 1 renderedImage.ppm
end_time="$(date -u +%s%3N)"
elapsed="$(($end_time-$start_time))"
inSecs=`bc <<< "scale=2; $elapsed / 1000"`
echo -e "Render time: $inSecs secs"
rm renderedImage.ppm

start_time="$(date -u +%s%3N)"
echo -e "./renderMain 1600 900 2 renderedImage.ppm | \c"
./renderMain 1600 900 2 renderedImage.ppm
end_time="$(date -u +%s%3N)"
elapsed="$(($end_time-$start_time))"
inSecs=`bc <<< "scale=2; $elapsed / 1000"`
echo -e "Render time: $inSecs secs"
rm renderedImage.ppm

start_time="$(date -u +%s%3N)"
echo -e "./renderMain 3840 2160 0 renderedImage.ppm | \c"
./renderMain 3840 2160 0 renderedImage.ppm
end_time="$(date -u +%s%3N)"
elapsed="$(($end_time-$start_time))"
inSecs=`bc <<< "scale=2; $elapsed / 1000"`
echo -e "Render time: $inSecs secs"
rm renderedImage.ppm

start_time="$(date -u +%s%3N)"
echo -e "./renderMain 3840 2160 1 renderedImage.ppm | \c"
./renderMain 3840 2160 1 renderedImage.ppm
end_time="$(date -u +%s%3N)"
elapsed="$(($end_time-$start_time))"
inSecs=`bc <<< "scale=2; $elapsed / 1000"`
echo -e "Render time: $inSecs secs"
rm renderedImage.ppm

start_time="$(date -u +%s%3N)"
echo -e "./renderMain 3840 2160 2 renderedImage.ppm | \c"
./renderMain 3840 2160 2 renderedImage.ppm
end_time="$(date -u +%s%3N)"
elapsed="$(($end_time-$start_time))"
inSecs=`bc <<< "scale=2; $elapsed / 1000"`
echo -e "Render time: $inSecs secs"
rm renderedImage.ppm

start_time="$(date -u +%s%3N)"
echo -e "./renderMain 3840 2160 3 renderedImage.ppm | \c"
./renderMain 3840 2160 3 renderedImage.ppm
end_time="$(date -u +%s%3N)"
elapsed="$(($end_time-$start_time))"
inSecs=`bc <<< "scale=2; $elapsed / 1000"`
echo -e "Render time: $inSecs secs"
rm renderedImage.ppm

start_time="$(date -u +%s%3N)"
echo -e "./renderMain 3840 2160 4 renderedImage.ppm | \c"
./renderMain 3840 2160 4 renderedImage.ppm
end_time="$(date -u +%s%3N)"
elapsed="$(($end_time-$start_time))"
inSecs=`bc <<< "scale=2; $elapsed / 1000"`
echo -e "Render time: $inSecs secs"
rm renderedImage.ppm

start_time="$(date -u +%s%3N)"
echo -e "./renderMain 7680 4320 0 renderedImage.ppm | \c"
./renderMain 7680 4320 0 renderedImage.ppm
end_time="$(date -u +%s%3N)"
elapsed="$(($end_time-$start_time))"
inSecs=`bc <<< "scale=2; $elapsed / 1000"`
echo -e "Render time: $inSecs secs"
rm renderedImage.ppm

start_time="$(date -u +%s%3N)"
echo -e "./renderMain 7680 4320 1 renderedImage.ppm | \c"
./renderMain 7680 4320 1 renderedImage.ppm
end_time="$(date -u +%s%3N)"
elapsed="$(($end_time-$start_time))"
inSecs=`bc <<< "scale=2; $elapsed / 1000"`
echo -e "Render time: $inSecs secs"
rm renderedImage.ppm

start_time="$(date -u +%s%3N)"
echo -e "./renderMain 7680 4320 2 renderedImage.ppm | \c"
./renderMain 7680 4320 2 renderedImage.ppm
end_time="$(date -u +%s%3N)"
elapsed="$(($end_time-$start_time))"
inSecs=`bc <<< "scale=2; $elapsed / 1000"`
echo -e "Render time: $inSecs secs"
rm renderedImage.ppm

start_time="$(date -u +%s%3N)"
echo -e "./renderMain 7680 4320 3 renderedImage.ppm | \c"
./renderMain 7680 4320 3 renderedImage.ppm
end_time="$(date -u +%s%3N)"
elapsed="$(($end_time-$start_time))"
inSecs=`bc <<< "scale=2; $elapsed / 1000"`
echo -e "Render time: $inSecs secs"
rm renderedImage.ppm

start_time="$(date -u +%s%3N)"
echo -e "./renderMain 7680 4320 4 renderedImage.ppm | \c"
./renderMain 7680 4320 4 renderedImage.ppm
end_time="$(date -u +%s%3N)"
elapsed="$(($end_time-$start_time))"
inSecs=`bc <<< "scale=2; $elapsed / 1000"`
echo -e "Render time: $inSecs secs"
rm renderedImage.ppm