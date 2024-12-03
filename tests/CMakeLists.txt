set(CONFIG_VARS_PATH "${CMAKE_SOURCE_DIR}/configs/config_vars.testing.yaml")
if (EXISTS "${CONFIG_VARS_PATH}")
    set(PYTEST_ARGS_CONFIG_VARS "--service-config-vars=${CONFIG_VARS_PATH}")
else()
    set(PYTEST_ARGS_CONFIG_VARS "")
endif()

userver_testsuite_add(
  SERVICE_TARGET ${PROJECT_NAME}
  REQUIREMENTS ${CMAKE_CURRENT_SOURCE_DIR}/requirements.txt
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  PYTEST_ARGS
  --service-config=${CMAKE_SOURCE_DIR}/configs/static_config.yaml
  --service-binary=${CMAKE_BINARY_DIR}/${PROJECT_NAME}
  --service-secdist=${CMAKE_SOURCE_DIR}/configs/jwt_config.json
  ${PYTEST_ARGS_CONFIG_VARS}
)
