SELECT P.firstName, P.lastName,
	CONDENSE +
	OVER i IN [L.lower1 : L.upper1, L.lower2 : L.upper2]
	USING Coll[i] + P.age
FROM (Persons AS P NATURAL JOIN IntegerLimits AS L),
	PhotoCollection AS Coll
