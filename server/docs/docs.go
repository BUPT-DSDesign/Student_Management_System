// Code generated by swaggo/swag. DO NOT EDIT.

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
        "/activity/add": {
            "post": {
                "security": [
                    {
                        "ApiKeyAuth": []
                    }
                ],
                "description": "增加活动信息",
                "produces": [
                    "application/json"
                ],
                "tags": [
                    "活动接口"
                ],
                "summary": "增加活动信息",
                "responses": {
                    "200": {
                        "description": "OK",
                        "schema": {
                            "$ref": "#/definitions/activity_handler.addResponse"
                        }
                    }
                }
            }
        },
        "/activity/delete": {
            "delete": {
                "security": [
                    {
                        "ApiKeyAuth": []
                    }
                ],
                "description": "删除活动信息",
                "produces": [
                    "application/json"
                ],
                "tags": [
                    "活动接口"
                ],
                "summary": "删除活动信息",
                "responses": {
                    "200": {
                        "description": "OK",
                        "schema": {
                            "$ref": "#/definitions/activity_handler.deleteResponse"
                        }
                    }
                }
            }
        },
        "/activity/info": {
            "get": {
                "security": [
                    {
                        "ApiKeyAuth": []
                    }
                ],
                "description": "获取学生所有的课外活动信息",
                "produces": [
                    "application/json"
                ],
                "tags": [
                    "活动接口"
                ],
                "summary": "课外活动信息",
                "responses": {
                    "200": {
                        "description": "OK",
                        "schema": {
                            "$ref": "#/definitions/activity_handler.getInfoResponse"
                        }
                    }
                }
            }
        },
        "/course/add": {
            "post": {
                "security": [
                    {
                        "ApiKeyAuth": []
                    }
                ],
                "description": "管理员添加课程",
                "produces": [
                    "application/json"
                ],
                "tags": [
                    "课程接口"
                ],
                "summary": "添加课程",
                "responses": {
                    "200": {
                        "description": "OK",
                        "schema": {
                            "$ref": "#/definitions/course_handler.addResponse"
                        }
                    }
                }
            }
        },
        "/course/delete": {
            "delete": {
                "security": [
                    {
                        "ApiKeyAuth": []
                    }
                ],
                "description": "根据id删除课程",
                "produces": [
                    "application/json"
                ],
                "tags": [
                    "课程接口"
                ],
                "summary": "删除课程",
                "responses": {
                    "200": {
                        "description": "OK",
                        "schema": {
                            "$ref": "#/definitions/course_handler.deleteResponse"
                        }
                    }
                }
            }
        },
        "/course/search": {
            "get": {
                "security": [
                    {
                        "ApiKeyAuth": []
                    }
                ],
                "description": "查询课程",
                "produces": [
                    "application/json"
                ],
                "tags": [
                    "课程接口"
                ],
                "summary": "查询课程",
                "responses": {
                    "200": {
                        "description": "OK",
                        "schema": {
                            "$ref": "#/definitions/course_handler.searchResponse"
                        }
                    }
                }
            }
        },
        "/course/table": {
            "get": {
                "security": [
                    {
                        "ApiKeyAuth": []
                    }
                ],
                "description": "获取学生所有的课程表信息",
                "produces": [
                    "application/json"
                ],
                "tags": [
                    "课程接口"
                ],
                "summary": "课程表",
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
        "/course/update": {
            "put": {
                "security": [
                    {
                        "ApiKeyAuth": []
                    }
                ],
                "description": "更新课程信息",
                "produces": [
                    "application/json"
                ],
                "tags": [
                    "课程接口"
                ],
                "summary": "更新课程",
                "responses": {
                    "200": {
                        "description": "OK",
                        "schema": {
                            "$ref": "#/definitions/course_handler.updateResponse"
                        }
                    }
                }
            }
        },
        "/log/add": {
            "post": {
                "security": [
                    {
                        "ApiKeyAuth": []
                    }
                ],
                "description": "添加一条日志",
                "consumes": [
                    "application/json"
                ],
                "produces": [
                    "application/json"
                ],
                "tags": [
                    "日志接口"
                ],
                "summary": "添加日志",
                "parameters": [
                    {
                        "type": "string",
                        "description": "时间",
                        "name": "create_time",
                        "in": "query",
                        "required": true
                    },
                    {
                        "type": "string",
                        "description": "内容",
                        "name": "content",
                        "in": "query",
                        "required": true
                    }
                ],
                "responses": {
                    "200": {
                        "description": "OK",
                        "schema": {
                            "$ref": "#/definitions/log_handler.addResponse"
                        }
                    }
                }
            }
        },
        "/log/delete": {
            "get": {
                "security": [
                    {
                        "ApiKeyAuth": []
                    }
                ],
                "description": "删除一条日志",
                "consumes": [
                    "application/json"
                ],
                "produces": [
                    "application/json"
                ],
                "tags": [
                    "日志接口"
                ],
                "summary": "删除日志",
                "parameters": [
                    {
                        "type": "string",
                        "description": "日志id",
                        "name": "log_id",
                        "in": "query",
                        "required": true
                    }
                ],
                "responses": {
                    "200": {
                        "description": "OK",
                        "schema": {
                            "$ref": "#/definitions/log_handler.deleteResponse"
                        }
                    }
                }
            }
        },
        "/log/info": {
            "get": {
                "security": [
                    {
                        "ApiKeyAuth": []
                    }
                ],
                "description": "查询日志",
                "consumes": [
                    "application/json"
                ],
                "produces": [
                    "application/json"
                ],
                "tags": [
                    "日志接口"
                ],
                "summary": "查询日志",
                "responses": {
                    "200": {
                        "description": "OK",
                        "schema": {
                            "$ref": "#/definitions/log_handler.infoResponse"
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
                    "导航接口"
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
        "/navigate/tsp": {
            "get": {
                "security": [
                    {
                        "ApiKeyAuth": []
                    }
                ],
                "description": "解决旅行商问题",
                "consumes": [
                    "application/json"
                ],
                "produces": [
                    "application/json"
                ],
                "tags": [
                    "导航接口"
                ],
                "summary": "旅行商",
                "parameters": [
                    {
                        "type": "integer",
                        "description": "开始节点ID",
                        "name": "start_id",
                        "in": "query",
                        "required": true
                    },
                    {
                        "type": "array",
                        "items": {
                            "type": "integer"
                        },
                        "collectionFormat": "csv",
                        "description": "途径节点集合",
                        "name": "des_ids",
                        "in": "query",
                        "required": true
                    }
                ],
                "responses": {
                    "200": {
                        "description": "OK",
                        "schema": {
                            "$ref": "#/definitions/navigate_handler.tspResponse"
                        }
                    }
                }
            }
        },
        "/polling/is_course_arrive": {
            "get": {
                "description": "课程是否来临",
                "consumes": [
                    "application/json"
                ],
                "produces": [
                    "application/json"
                ],
                "tags": [
                    "轮询接口"
                ],
                "summary": "课程是否来临",
                "responses": {
                    "200": {
                        "description": "OK",
                        "schema": {
                            "$ref": "#/definitions/polling_handler.isCourseArriveResponse"
                        }
                    }
                }
            }
        },
        "/user/info": {
            "get": {
                "description": "获取用户信息",
                "consumes": [
                    "application/json"
                ],
                "produces": [
                    "application/json"
                ],
                "tags": [
                    "用户接口"
                ],
                "summary": "用户信息",
                "responses": {
                    "200": {
                        "description": "OK",
                        "schema": {
                            "$ref": "#/definitions/user_handler.infoResponse"
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
                    "用户接口"
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
                    "用户接口"
                ],
                "summary": "注册",
                "parameters": [
                    {
                        "description": "用户信息",
                        "name": "user_input",
                        "in": "body",
                        "required": true,
                        "schema": {
                            "$ref": "#/definitions/common.RegisterRequest"
                        }
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
        },
        "/user/upload_avatar": {
            "post": {
                "description": "用户上传头像",
                "consumes": [
                    "application/json"
                ],
                "produces": [
                    "application/json"
                ],
                "tags": [
                    "用户接口"
                ],
                "summary": "上传头像",
                "responses": {
                    "200": {
                        "description": "OK",
                        "schema": {
                            "$ref": "#/definitions/user_handler.uploadAvatarResponse"
                        }
                    }
                }
            }
        }
    },
    "definitions": {
        "activity_handler.addResponse": {
            "type": "object",
            "properties": {
                "status_code": {
                    "type": "integer"
                },
                "status_msg": {
                    "type": "string"
                }
            }
        },
        "activity_handler.deleteResponse": {
            "type": "object",
            "properties": {
                "status_code": {
                    "type": "integer"
                },
                "status_msg": {
                    "type": "string"
                }
            }
        },
        "activity_handler.getInfoResponse": {
            "type": "object",
            "properties": {
                "activities": {
                    "type": "array",
                    "items": {
                        "$ref": "#/definitions/system.ActivityInfo"
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
        "common.RegisterRequest": {
            "type": "object",
            "properties": {
                "password": {
                    "type": "string"
                },
                "student_id": {
                    "type": "string"
                },
                "username": {
                    "type": "string"
                }
            }
        },
        "course_handler.addResponse": {
            "type": "object",
            "properties": {
                "status_code": {
                    "type": "integer"
                },
                "status_msg": {
                    "type": "string"
                }
            }
        },
        "course_handler.deleteResponse": {
            "type": "object",
            "properties": {
                "status_code": {
                    "type": "integer"
                },
                "status_msg": {
                    "type": "string"
                }
            }
        },
        "course_handler.searchResponse": {
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
        "course_handler.updateResponse": {
            "type": "object",
            "properties": {
                "status_code": {
                    "type": "integer"
                },
                "status_msg": {
                    "type": "string"
                }
            }
        },
        "log_handler.addResponse": {
            "type": "object",
            "properties": {
                "log_id": {
                    "type": "integer"
                },
                "status_code": {
                    "type": "integer"
                },
                "status_msg": {
                    "type": "string"
                }
            }
        },
        "log_handler.deleteResponse": {
            "type": "object",
            "properties": {
                "status_code": {
                    "type": "integer"
                },
                "status_msg": {
                    "type": "string"
                }
            }
        },
        "log_handler.infoResponse": {
            "type": "object",
            "properties": {
                "logs": {
                    "type": "array",
                    "items": {
                        "$ref": "#/definitions/system.LogInfo"
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
                "path_list": {
                    "type": "array",
                    "items": {
                        "$ref": "#/definitions/system.PathGuider"
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
        "navigate_handler.tspResponse": {
            "type": "object",
            "properties": {
                "node_list": {
                    "description": "途径节点的经纬度",
                    "type": "array",
                    "items": {
                        "type": "array",
                        "items": {
                            "type": "number"
                        }
                    }
                },
                "pass_list": {
                    "description": "用户选择的目标点的经纬度，用户在地图上显示",
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
        "polling_handler.isCourseArriveResponse": {
            "type": "object",
            "properties": {
                "course_info": {
                    "$ref": "#/definitions/system.CourseInfo"
                },
                "is_arrive": {
                    "type": "boolean"
                },
                "status_code": {
                    "type": "integer"
                },
                "status_msg": {
                    "type": "string"
                }
            }
        },
        "system.ActivityInfo": {
            "type": "object",
            "properties": {
                "activity_id": {
                    "type": "integer"
                },
                "activity_name": {
                    "type": "string"
                },
                "frequency": {
                    "type": "integer"
                },
                "is_mention": {
                    "description": "是否需要提醒",
                    "type": "boolean"
                },
                "location": {
                    "type": "string"
                },
                "start_time": {
                    "type": "string"
                },
                "tag": {
                    "type": "string"
                },
                "type": {
                    "type": "integer"
                },
                "user_id": {
                    "type": "integer"
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
                "is_compulsory": {
                    "description": "课程是否必修",
                    "type": "boolean"
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
        "system.LogInfo": {
            "type": "object",
            "properties": {
                "content": {
                    "type": "string"
                },
                "create_time": {
                    "type": "string"
                },
                "log_id": {
                    "type": "integer"
                },
                "user_id": {
                    "description": "对应的用户id, 对应某个用户的日志",
                    "type": "integer"
                }
            }
        },
        "system.PathGuider": {
            "type": "object",
            "properties": {
                "distance": {
                    "description": "向量长度,单位为米",
                    "type": "number"
                },
                "heading": {
                    "description": "向量方向,用航向角来计算(正北为0,360等分)",
                    "type": "integer"
                }
            }
        },
        "system.UserInfo": {
            "type": "object",
            "properties": {
                "avatar_url": {
                    "description": "保存用户头像的路径",
                    "type": "string"
                },
                "is_admin": {
                    "type": "boolean"
                },
                "signature": {
                    "description": "......后续应该还要加上一些字段",
                    "type": "string"
                },
                "student_id": {
                    "type": "string"
                },
                "user_id": {
                    "type": "integer"
                },
                "username": {
                    "type": "string"
                }
            }
        },
        "user_handler.infoResponse": {
            "type": "object",
            "properties": {
                "status_code": {
                    "type": "integer"
                },
                "status_msg": {
                    "type": "string"
                },
                "user_info": {
                    "$ref": "#/definitions/system.UserInfo"
                }
            }
        },
        "user_handler.loginResponse": {
            "type": "object",
            "properties": {
                "is_admin": {
                    "type": "boolean"
                },
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
                "is_admin": {
                    "type": "boolean"
                },
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
        "user_handler.uploadAvatarResponse": {
            "type": "object",
            "properties": {
                "avatar_url": {
                    "type": "string"
                },
                "status_code": {
                    "type": "integer"
                },
                "status_msg": {
                    "type": "string"
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
	LeftDelim:        "{{",
	RightDelim:       "}}",
}

func init() {
	swag.Register(SwaggerInfo.InstanceName(), SwaggerInfo)
}
