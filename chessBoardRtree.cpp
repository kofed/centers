#include "chessBoardRtree.h"

ChessBoardRtree::ChessBoardRtree(const std::vector<std::vector<Point2f>> & points, const Size _size,
		const Point2f _center, const Point2i _centerIndex, const float _cellSize, const Size _resolution)
	:ChessBoard(points, _size, _center, _centerIndex, _cellSize, _resolution){

}

void ChessBoardRtree::setValue(const Point2i index, float value){
	Point2f corner = get(index);

	float x1, y1, x2, y2;
	if(index.x != 0){
		x1 = get(index.x - 1, index.y).x;
	}else{
		x1 = 0;
	}

	if(index.y != 0){
		y1 = get(index.x, index.y - 1).y;
	}else{
		y1 = 0;
	}

	if(index.x != size.width - 1){
		x2 = get(index.x + 1, index.y).x;
	}else{
		x2 = resolution.width;
	}

	if(index.y != size.height - 1){
		y2 = get(index.x, index.y + 1).y;
	}else{
		y2 = resolution.height;
	}

	box b(point((corner.x+x1)/2, (corner.y+y1)/2), point((corner.x+x2)/2, (corner.y+y2)/2));

    rtree.insert(std::make_pair(b, value));
    indexRtree.insert(std::make_pair(b, index));
}

Point2i ChessBoardRtree::nearest(const Point2f p) const{
	std::vector<IndexValue> result;
	indexRtree.query(bgi::nearest(point(p.x, p.y), 1), std::back_inserter(result));
	return result.at(0).second;
}

float ChessBoardRtree::getValue(const Point2f p) const{
	std::vector<value> result;
		rtree.query(bgi::nearest(point(p.x, p.y), 1), std::back_inserter(result));
		return result.at(0).second;
}

void operator>>(const FileNode & fn, ChessBoardRtree & cb){

	}
FileStorage & operator<<(FileStorage & fs, const point & p);
FileStorage & operator<<(FileStorage & fs, const point & p){
	fs << "{";
	fs << "x" << p.get<0>();
	fs << "y" << p.get<1>();
	fs << "}";
	return fs;
}

FileStorage & operator<<( FileStorage & fs, const ChessBoardRtree & cb){
	fs << "boxes" << "[";
	for(auto val = cb.rtree.begin(); val != cb.rtree.end(); ++val){
		fs <<  "{";
	//	//toTxt(fs, val->first.min_corner);
		fs << "min_point" << val->first.min_corner();
		fs << "max_point" << val->first.max_corner();
		float s = val->second;
		fs << "disparity" << s;// ;
		fs << "}";
	}
	fs << "]";

	    return fs;
}




