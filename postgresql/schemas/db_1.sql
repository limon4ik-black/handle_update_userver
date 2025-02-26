CREATE EXTENSION IF NOT EXISTS citext;
CREATE EXTENSION IF NOT EXISTS pgcrypto;

DROP SCHEMA IF EXISTS robinid CASCADE;
CREATE SCHEMA IF NOT EXISTS robinid;

CREATE TABLE IF NOT EXISTS robinid.users (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    username CITEXT UNIQUE NOT NULL,
    email VARCHAR(255) NOT NULL,
    password_hash VARCHAR(255) NOT NULL
);


CREATE TYPE robinid.user AS (
    id TEXT,
    username CITEXT,
    email VARCHAR(255),
    password_hash VARCHAR(255)
);


CREATE OR REPLACE FUNCTION robinid.add_new_user(
    _username CITEXT,
    _email VARCHAR(255),
    _password_hash VARCHAR(255)
)
RETURNS TEXT
AS $$
DECLARE
    _user_id TEXT;
BEGIN
    INSERT INTO robinid.users (username, email, password_hash)
    VALUES (_username, _email, _password_hash)
    RETURNING id INTO _user_id;

    RETURN _user_id::TEXT;
END;
$$ LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION robinid.get_user_by_username(
    _username CITEXT
)
RETURNS SETOF robinid.user
AS $$
BEGIN
    RETURN QUERY
    SELECT
        id::TEXT,
        username,
        email,
        password_hash
    FROM
        robinid.users
    WHERE
        username = _username;
END;
$$ LANGUAGE plpgsql;


-- CREATE OR REPLACE FUNCTION robinid.update_user(
--     _username CITEXT,
--     _new_username CITEXT DEFAULT NULL,
--     _new_email VARCHAR(255) DEFAULT NULL,
--     _new_password_hash VARCHAR(255) DEFAULT NULL
-- )
-- RETURNS TEXT
-- AS $$
-- DECLARE
--     _user_id TEXT;
-- BEGIN

--     SELECT id::TEXT
--     INTO _user_id
--     FROM robinid.users
--     WHERE username = _username;

--     IF _user_id IS NULL THEN
--         RAISE EXCEPTION 'User with username "%" not found', _username;
--     END IF;

--     IF _new_username IS NOT NULL THEN
--         UPDATE robinid.users
--         SET username = _new_username
--         WHERE id = _user_id::UUID;
--     END IF;

--     IF _new_email IS NOT NULL THEN
--         UPDATE robinid.users
--         SET email = _new_email
--         WHERE id = _user_id::UUID;
--     END IF;

--     IF _new_password_hash IS NOT NULL THEN
--         UPDATE robinid.users
--         SET password_hash = _new_password_hash
--         WHERE id = _user_id::UUID;
--     END IF;

--     RETURN _user_id;
-- END;
-- $$ LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION robinid.update_user(
	_user_id text,
	_new_username citext DEFAULT NULL::citext,
	_new_email character varying DEFAULT NULL::character varying,
	_new_password_hash character varying DEFAULT NULL::character varying)
    RETURNS text
    LANGUAGE 'plpgsql'
    COST 100
    VOLATILE PARALLEL UNSAFE
AS $BODY$
BEGIN
    -- Проверка существования пользователя с переданным _user_id
    IF NOT EXISTS (
        SELECT 1
        FROM robinid.users
        WHERE id = _user_id::uuid
    ) THEN
        RAISE EXCEPTION 'User with id "%" not found', _user_id;
    END IF;

    -- Обновление username, если передано новое значение
    IF _new_username IS NOT NULL THEN
        UPDATE robinid.users
        SET username = _new_username
        WHERE id = _user_id::uuid;
    END IF;

    -- Обновление email, если передано новое значение
    IF _new_email IS NOT NULL THEN
        UPDATE robinid.users
        SET email = _new_email
        WHERE id = _user_id::uuid;
    END IF;

    -- Обновление password_hash, если передано новое значение
    IF _new_password_hash IS NOT NULL THEN
        UPDATE robinid.users
        SET password_hash = _new_password_hash
        WHERE id = _user_id::uuid;
    END IF;

    -- Возврат _user_id
    RETURN _user_id;
END;
$BODY$;
