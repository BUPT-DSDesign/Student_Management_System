<template>
    <div class="Aside">
        <el-menu default-active="1" class="el-menu-vertical-demo" @open="handleOpen" @close="handleClose"
            :collapse="isCollapse" background-color="#545c64" text-color="#fff" active-text-color="#ffd04b">
            <h3 class="title">学生日程管理系统</h3>
            <el-menu-item index="1" @click="clickMain">
                <i class="el-icon-user-solid"></i>
                <span slot="title">个人主页</span>
            </el-menu-item>
            <el-submenu index="2">
                <template slot="title">
                    <i class="el-icon-date"></i>
                    <span slot="title">日程管理</span>
                </template>
                <el-menu-item-group>
                    <el-menu-item index="2-1" @click="clickinClass">课内信息管理</el-menu-item>
                </el-menu-item-group>
                <el-menu-item-group>
                    <el-menu-item index="2-2" @click="clickoutClass">课外活动管理</el-menu-item>
                </el-menu-item-group>
            </el-submenu>
            <el-menu-item index="3" @click="clickCourseNav">
                <i class="el-icon-location"></i>
                <span slot="title">课程导航</span>
            </el-menu-item>
            <el-menu-item index="4" @click="clickLogMessage">
                <i class="el-icon-news"></i>
                <span slot="title">日志信息</span>
            </el-menu-item>
        </el-menu>
    </div>
</template>

<script>
import { CourseStore } from '@/store/course'
import { EventStore } from '@/store/event'
import { LogStore } from '@/store/log'                

export default {
    data() {
        return {
            isCollapse: false,
        };
    },
    methods: {
        handleOpen(key, keyPath) {
            console.log(key, keyPath);
        },
        handleClose(key, keyPath) {
            console.log(key, keyPath);
        },
        clickMain() {
            this.$router.push('/Main/Homepage');
        },
        //点击侧边菜单
        clickCourseNav() {
            this.$router.push('/Main/CourseNav');
        },

        getCourseTable: async function () {
            return await CourseStore.GetCourseTable()
        },
        clickinClass() {
            // 当点击侧边栏的课内信息时, 会向后端发送请求, 后端返回课程表
            const getTable1 = async () => {
                const fg = await this.getCourseTable()
                if (fg) {
                    this.$router.push('/Main/inClass');
                } else {
                    console.log('error')
                }
            }
            getTable1()
        },
        getEventTable: async function () {
            return await EventStore.GetEventTable()
        },
        clickoutClass() {
            // 当点击侧边栏的课外信息时, 会向后端发送请求, 后端返回课外活动表
            const getTable2 = async () => {
                const fg = await this.getEventTable()
                if (fg) {
                    this.$router.push('/Main/outClass');
                } else {
                    console.log('error')
                }
            }
            getTable2()
        },
        clickLogMessage() {
            // 当点击日志信息时, 会向后端发送请求, 后端返回日志信息
            const getLogMessage = async () => {
                const fg = await LogStore.GetLogMessage()
                if (fg) {
                    console.log(LogStore.logs)
                    this.$router.push('/Main/logMessage')
                } else {
                    console.log('error')
                }
            }
            getLogMessage()
        }
    }
}
</script>

<style>
* {
    margin: 0;
    padding: 0;
}

html, body {
    height: 100%;
}

.Aside {
    height: 100%;
}

.el-menu-vertical-demo:not(.el-menu--collapse) {
    width: 200px;
    height: 100%;
}

.el-aside {
    overflow-x: hidden;
}

h3 {
    margin: 15px;
    font-size: 18px;
    text-align: center;
    color: #fff;
}
</style>