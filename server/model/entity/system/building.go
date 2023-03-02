package system

type Coordinate struct {
	Longitude float64
	Latitude  float64
}
type Building struct {
	Id int
	Coordinate
}
