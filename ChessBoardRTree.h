class ChessBoardRtree : ChessBoard{
private:
	Rtree rtree;
public:	
	ChessBoardRtree():ChessBoard(){
	
	}

	void set(Point2i index, float value);
	
	Point2i nearest(const Point2f point);
};