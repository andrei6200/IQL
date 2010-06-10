SELECT P.firstName, P.lastName, R.rating, Coll [1:100, 200:500]
FROM PhotoCollection AS Coll, 
	Persons AS P JOIN Ratings AS R
	ON P.lastName = R.lastName AND P.firstName != R.lastName
