#Shared Pixel Libs:
MYPIXGEO_SHL = MyPixelGeoDescr.so
MYPIXGEO_OBJ = build/MyPixelGeoDescr.o build/Rectangle.o

MYOTHERPIXGEO_SHL = MyOtherPixelGeoDescr.so
MYOTHERPIXGEO_OBJ = build/MyOtherPixelGeoDescr.o build/Rectangle.o

LONGEDGEPIXELGEO_SHL = LongEdgePixelGeo.so
LONGEDGEPIXELGEO_OBJ = build/LongEdgePixelGeo.o build/Rectangle.o

SMALLLGEO_SHL = SmallLGeo.so
SMALLLGEO_OBJ = build/SmallLGeo.o build/Rectangle.o

STRIPGEO_SHL = StripGeo.so
STRIPGEO_OBJ = build/StripGeo.o build/Rectangle.o

CXX      = g++
CXXFLAGS = -Wall -std=c++11 -fPIC
LDFLAGS  = -ldl 

all: $(MYPIXGEO_SHL) $(MYOTHERPIXGEO_SHL) $(LONGEDGEPIXELGEO_SHL) $(SMALLLGEO_SHL) $(STRIPGEO_SHL)

.PHONY: clean all

clean: 
	rm -rf build/*

#Geo shared libs
$(MYPIXGEO_SHL): $(MYPIXGEO_OBJ)
	$(CXX) -shared -o build/$@ $^

$(MYOTHERPIXGEO_SHL): $(MYOTHERPIXGEO_OBJ)
	$(CXX) -shared -o build/$@ $^

$(LONGEDGEPIXELGEO_SHL): $(LONGEDGEPIXELGEO_OBJ)
	$(CXX) -shared -o build/$@ $^

$(SMALLLGEO_SHL):  $(SMALLLGEO_OBJ)
	$(CXX) -shared -o build/$@ $^

 $(STRIPGEO_SHL):  $(STRIPGEO_OBJ)
	$(CXX) -shared -o build/$@ $^

# --- object dependencies ---

build/MyPixelGeoDescr.o: MyPixelGeoDescr.cpp MyPixelGeoDescr.h PixelGeoDescr.h Shape.h Rectangle.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

build/MyOtherPixelGeoDescr.o: MyOtherPixelGeoDescr.cpp MyOtherPixelGeoDescr.h PixelGeoDescr.h Shape.h Rectangle.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

build/LongEdgePixelGeo.o: LongEdgePixelGeo.cpp LongEdgePixelGeo.h PixelGeoDescr.h Shape.h Rectangle.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

build/SmallLGeo.o: SmallLGeo.cpp SmallLGeo.h PixelGeoDescr.h Shape.h Rectangle.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

build/StripGeo.o: StripGeo.cpp StripGeo.h PixelGeoDescr.h Shape.h Rectangle.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

build/Rectangle.o: Rectangle.cpp Rectangle.h Shape.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<
