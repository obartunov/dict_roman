SET search_path = public;
 create or replace  function dinit_roman(internal)
         returns internal
         as 'MODULE_PATHNAME'
         language C;

 create or replace  function dlexize_roman(internal,internal,internal, internal)
        returns internal
        as 'MODULE_PATHNAME'
        language C STRICT;

CREATE TEXT SEARCH TEMPLATE roman_template (
        LEXIZE = 'dlexize_roman',
            INIT   = 'dinit_roman'
);

CREATE TEXT SEARCH DICTIONARY roman (
           TEMPLATE = roman_template
);

COMMENT ON TEXT SEARCH DICTIONARY roman IS 'Dictionary for roman dates';
