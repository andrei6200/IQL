SELECT
	CONDENSE +
	OVER i IN [1:100,1:100]
	VALUES Coll[i]
FROM PhotoCollection AS Coll