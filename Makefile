#Programs:
MYPROG_EXE = MyProg
MYPROG_SRC = MyProg.cpp

DH_EXE = DrawHit
DH_SRC = DrawHit.cpp
DH_OBJ = build/DrawHit.o build/EventDefines_h.so

MC_EXE = MyMcGenerator
MC_SRC = MyMcGenerator.cpp
MC_OBJ = geolibs/build/Rectangle.o build/EventDefines_h.so

ETACLU_EXE = EtaClustering
ETACLU_SRC = EtaClustering.cpp
ETACLU_OBJ = build/EtaClustering.o build/EventDefines_h.so

CXX      = g++
CXXFLAGS = -Wall -std=c++11 -fPIC -Igeolibs -Lgeolibs $(shell root-config --cflags)
LDFLAGS  = -ldl $(shell root-config --libs) 

all:
	make -C geolibs all
	make dw
	make mc 
	make dh
	make eta

dw:  $(MYPROG_EXE)

mc: $(MC_OBJ) $(MC_EXE)

dh: $(DH_OBJ) $(DH_EXE)

eta: $(ETACLU_OBJ) $(ETACLU_EXE)

.PHONY: clean all

clean: 
	rm -rf exe/*
	rm -rf output/*
	mkdir temp 
	cp build/EventDefines_h.so temp/EventDefines_h.so
	rm -rf build/*
	cp temp/EventDefines_h.so build/EventDefines_h.so
	rm -rf temp

#Program EXEs:
$(MYPROG_EXE): $(MYPROG_SRC)
	$(CXX) $(CXXFLAGS) -o exe/$@ $^ $(LDFLAGS)

$(MC_EXE): $(MC_SRC)
	$(CXX) $(CXXFLAGS) -o exe/$@ $^ $(MC_OBJ) $(LDFLAGS)

$(DH_EXE): $(DH_OBJ)
	$(CXX) $(CXXFLAGS) -o exe/$@ $^ $(LDFLAGS)

$(ETACLU_EXE): $(ETACLU_OBJ)
	$(CXX) $(CXXFLAGS) -o exe/$@ $^ $(LDFLAGS) 

# --- object dependencies ---

build/MyProg.o: MyProg.cpp geolibs/PixelGeoDescr.h geolibs/Shape.h geolibs/SvgDefines.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

build/MyMcGenerator.o: MyProg.cpp geolibs/PixelGeoDescr.h Shape.h Rectangle.h SvgDefines.h EventDefines.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

build/DrawHit.o: DrawHit.cpp geolibs/PixelGeoDescr.h SvgDefines.h ReadEvent.h EventDefines.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

build/EtaClustering.o: EtaClustering.cpp geolibs/PixelGeoDescr.h EventDefines.h ReadEvent.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

