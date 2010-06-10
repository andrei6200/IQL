SELECT
	CONDENSE +
	OVER i IN [1:100,1:100]
	USING Coll[i]
FROM PhotoCollection AS Coll
