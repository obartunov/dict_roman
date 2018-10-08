CREATE EXTENSION dict_roman;

--lexize
SELECT ts_lexize('roman','X');
SELECT ts_lexize('roman','123');