SELECT P.firstName, P.lastName, R.rating,
	CONDENSE +
	OVER i IN [1:100, 1:100]
	USING Coll[i]
FROM PhotoCollection AS Coll, 
	Persons AS P JOIN Ratings AS R
	ON P.lastName = R.lastName AND P.firstName != R.lastName
