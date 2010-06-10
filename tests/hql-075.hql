SELECT P.firstName, P.lastName, R.rating, scale(Coll, [2,3])
FROM PhotoCollection AS Coll, 
	Persons AS P JOIN Ratings AS R
	ON P.lastName = R.lastName AND P.firstName != R.lastName
