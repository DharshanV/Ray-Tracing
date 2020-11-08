FROM rikorose/gcc-cmake:latest

COPY . /usr/src/RayTracer

WORKDIR /usr/src/RayTracer

RUN mkdir build
WORKDIR /usr/src/RayTracer/build
RUN cmake ..
RUN make
WORKDIR /usr/src/RayTracer

CMD [ "./build/render","500","500","out.ppm" ]