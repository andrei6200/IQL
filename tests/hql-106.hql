SELECT P.firstName, P.lastName,
	CONDENSE +
	OVER i IN [1:100, 1:100]
	USING Coll[i]

	INTO sqlTable
FROM Persons AS P, PhotoCollection AS Coll
