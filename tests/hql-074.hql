SELECT P.firstName, P.lastName, R.rating, 255 - Coll.red
FROM PhotoCollection AS Coll, 
	Persons AS P JOIN Ratings AS R
	ON P.lastName = R.lastName AND P.firstName != R.lastName
