// Code generated by swaggo/swag. DO NOT EDIT
package docs

import "github.com/swaggo/swag"

const docTemplate = `{
    "schemes": {{ marshal .Schemes }},
    "swagger": "2.0",
    "info": {
        "description": "{{escape .Description}}",
        "title": "{{.Title}}",
        "termsOfService": "http://swagger.io/terms/",
        "contact": {
            "name": "API Support",
            "url": "http://www.swagger.io/support",
            "email": "support@swagger.io"
        },
        "license": {
            "name": "Apache 2.0",
            "url": "http://www.apache.org/licenses/LICENSE-2.0.html"
        },
        "version": "{{.Version}}"
    },
    "host": "{{.Host}}",
    "basePath": "{{.BasePath}}",
    "paths": {
        "/course/table": {
            "get": {
                "security": [
                    {
                        "ApiKeyAuth": []
                    }
                ],
                "description": "获取学生所有的课程表信息",
                "consumes": [
                    "application/json"
                ],
                "produces": [
                    "application/json"
                ],
                "tags": [
                    "课程表相关接口"
                ],
                "summary": "课程表",
                "parameters": [
                    {
                        "type": "string",
                        "description": "学生ID",
                        "name": "user_id",
                        "in": "query",
                        "required": true
                    }
                ],
                "responses": {
                    "200": {
                        "description": "OK",
                        "schema": {
                            "$ref": "#/definitions/course_handler.tableResponse"
                        }
                    }
                }
            }
        },
        "/navigate/path": {
            "get": {
                "security": [
                    {
                        "ApiKeyAuth": []
                    }
                ],
                "description": "获取导航路线",
                "consumes": [
                    "application/json"
                ],
                "produces": [
                    "application/json"
                ],
                "tags": [
                    "导航功能"
                ],
                "summary": "导航",
                "parameters": [
                    {
                        "type": "integer",
                        "description": "起始节点ID",
                        "name": "from_id",
                        "in": "query",
                        "required": true
                    },
                    {
                        "type": "integer",
                        "description": "结束节点ID",
                        "name": "des_id",
                        "in": "query",
                        "required": true
                    }
                ],
                "responses": {
                    "200": {
                        "description": "OK",
                        "schema": {
                            "$ref": "#/definitions/navigate_handler.NavigateResponse"
                        }
                    }
                }
            }
        },
        "/user/login": {
            "post": {
                "description": "用户登录",
                "consumes": [
                    "application/json"
                ],
                "produces": [
                    "application/json"
                ],
                "tags": [
                    "用户鉴权"
                ],
                "summary": "登录",
                "parameters": [
                    {
                        "description": "用户信息",
                        "name": "user_input",
                        "in": "body",
                        "required": true,
                        "schema": {
                            "$ref": "#/definitions/common.LoginRequest"
                        }
                    }
                ],
                "responses": {
                    "200": {
                        "description": "OK",
                        "schema": {
                            "$ref": "#/definitions/user_handler.loginResponse"
                        }
                    }
                }
            }
        },
        "/user/register": {
            "post": {
                "description": "用户注册",
                "consumes": [
                    "application/json"
                ],
                "produces": [
                    "application/json"
                ],
                "tags": [
                    "用户鉴权"
                ],
                "summary": "注册",
                "parameters": [
                    {
                        "type": "string",
                        "description": "学号",
                        "name": "student_id",
                        "in": "query",
                        "required": true
                    },
                    {
                        "type": "string",
                        "description": "用户名",
                        "name": "username",
                        "in": "query",
                        "required": true
                    },
                    {
                        "type": "string",
                        "description": "密码",
                        "name": "password",
                        "in": "query",
                        "required": true
                    }
                ],
                "responses": {
                    "200": {
                        "description": "OK",
                        "schema": {
                            "$ref": "#/definitions/user_handler.registerResponse"
                        }
                    }
                }
            }
        }
    },
    "definitions": {
        "common.LoginRequest": {
            "type": "object",
            "properties": {
                "password": {
                    "type": "string"
                },
                "username": {
                    "type": "string"
                }
            }
        },
        "course_handler.tableResponse": {
            "type": "object",
            "properties": {
                "course_list": {
                    "type": "array",
                    "items": {
                        "$ref": "#/definitions/system.CourseInfo"
                    }
                },
                "status_code": {
                    "type": "integer"
                },
                "status_msg": {
                    "type": "string"
                }
            }
        },
        "navigate_handler.NavigateResponse": {
            "type": "object",
            "properties": {
                "node_list": {
                    "type": "array",
                    "items": {
                        "type": "array",
                        "items": {
                            "type": "number"
                        }
                    }
                },
                "status_code": {
                    "type": "integer"
                },
                "status_msg": {
                    "type": "string"
                }
            }
        },
        "system.CourseInfo": {
            "type": "object",
            "properties": {
                "classroom": {
                    "description": "线下课程记录教室, 线上课程记录会议链接",
                    "type": "string"
                },
                "contact": {
                    "type": "string"
                },
                "course_id": {
                    "type": "integer"
                },
                "course_location_node": {
                    "description": "对应导航地图的节点, 设置成数组是因为一个地点可能对应多个node",
                    "type": "array",
                    "items": {
                        "type": "integer"
                    }
                },
                "course_name": {
                    "type": "string"
                },
                "exam_location": {
                    "type": "string"
                },
                "exam_location_node": {
                    "type": "array",
                    "items": {
                        "type": "integer"
                    }
                },
                "exam_option": {
                    "type": "integer"
                },
                "exam_time": {
                    "description": "考试时间, 格式为'第几周-星期几-几点~几点（HH:mm~HH:mm）', 如'第十六周-星期三-19:00~20:00'",
                    "type": "string"
                },
                "is_course_online": {
                    "type": "boolean"
                },
                "section_list": {
                    "type": "array",
                    "items": {
                        "type": "integer"
                    }
                },
                "teacher": {
                    "type": "string"
                },
                "week_schedule": {
                    "type": "array",
                    "items": {
                        "type": "integer"
                    }
                }
            }
        },
        "user_handler.loginResponse": {
            "type": "object",
            "properties": {
                "status_code": {
                    "type": "integer"
                },
                "status_msg": {
                    "type": "string"
                },
                "token": {
                    "type": "string"
                },
                "user_id": {
                    "type": "integer"
                }
            }
        },
        "user_handler.registerResponse": {
            "type": "object",
            "properties": {
                "status_code": {
                    "type": "integer"
                },
                "status_msg": {
                    "type": "string"
                },
                "token": {
                    "type": "string"
                },
                "user_id": {
                    "type": "integer"
                }
            }
        }
    },
    "securityDefinitions": {
        "ApiKeyAuth": {
            "type": "apiKey",
            "name": "token",
            "in": "header"
        }
    }
}`

// SwaggerInfo holds exported Swagger Info so clients can modify it
var SwaggerInfo = &swag.Spec{
	Version:          "alpha 1.0",
	Host:             "localhost:8080",
	BasePath:         "/Student_Management_System",
	Schemes:          []string{},
	Title:            "Student Management System API",
	Description:      "A application for Data Structure Lesson",
	InfoInstanceName: "swagger",
	SwaggerTemplate:  docTemplate,
}

func init() {
	swag.Register(SwaggerInfo.InstanceName(), SwaggerInfo)
}
