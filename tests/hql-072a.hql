SELECT P.firstName, P.lastName, R.rating, Coll
FROM PhotoCollection AS Coll, 
	Persons AS P JOIN Ratings AS R
	ON P.lastName = R.lastName AND P.firstName != R.lastName
WHERE avg_cells(Coll) > 128
