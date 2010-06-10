SELECT P.firstName, P.lastName, R.rating,
	MARRAY x IN [1:100],
	       y IN [20:40]
	VALUES Coll[x,y]
FROM PhotoCollection AS Coll, 
	Persons AS P JOIN Ratings AS R
	ON P.lastName = R.lastName AND P.firstName != R.lastName
