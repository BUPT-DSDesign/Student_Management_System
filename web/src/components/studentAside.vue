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
                <span slot="title">导航</span>
            </el-menu-item>
            <el-menu-item index="4" @click="clickLogMessage">
                <i class="el-icon-news"></i>
                <span slot="title">日志信息</span>
            </el-menu-item>
            <el-dialog title="闹钟提醒" :visible="showClassAlarm" :modal="false" :custom-class="'popup'"
                @close="showClassAlarm = false">
                <div>
                    <p>{{ pollingCourse.course_name }}课程马上开始了</p>
                    <el-button type="primary" icon="el-icon-location" @click="goToNavigation">开始导航</el-button>
                </div>
            </el-dialog>
            <el-dialog title="闹钟提醒" :visible="showEventAlarm" :modal="false" :custom-class="'popup'"
                @close="showEventAlarm = false">
                <div>
                    <p>{{ pollingEvent.activity_name }}活动马上开始了</p>
                    <el-button type="primary" icon="el-icon-location" @click="goToNavigation">开始导航</el-button>
                </div>
            </el-dialog>
        </el-menu>
    </div>
</template>

<script>
import { TimeStore } from "@/store/time"
import { PollingStore } from "@/store/polling"

export default {
    data() {
        return {
            speed: TimeStore.Tm, //倍速
            isCollapse: false,
            is_arrive: false,
            pollingCourse: {},
            pollingEvent: {},
            showClassAlarm: true,
            showEventAlarm: true
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
            this.$router.push('/studentMain/Homepage');
        },
        //点击侧边菜单
        clickCourseNav() {
            this.$router.push('/studentMain/CourseNav');
        },
        clickinClass() {
            this.$router.push('/studentMain/inClass');
        },
        clickoutClass() {
            this.$router.push('/studentMain/outClass')
        },
        clickLogMessage() {
            this.$router.push('/studentMain/logMessage')
        },
        goToNavigation() {
            this.$router.push('/studentMain/CourseNav');
        },

        async getEventPolling() {
            const fg = await PollingStore.IsEventArrive();
            if (fg) {
                if (PollingStore.is_arrive == true) {
                    this.showAlarm = true
                    this.pollingEvent = PollingStore.pollingEvent;
                }

            } else {
                console.log('error')
            }
        }
    },
    mounted() {
        setInterval(async () => {
            const fg = await PollingStore.GetArrivedCourse(TimeStore.getTime());
            if (fg) {
                if (PollingStore.is_arrive == true) {
                    this.showAlarm = true
                    this.pollingCourse = PollingStore.pollingCourse;
                }
            } else {
                // console.log('error')
            }
        }, 5000);
        setInterval(async () => {
            const fg = await PollingStore.GetArrivedEvent(TimeStore.getTime());
            if (fg) {
                if (PollingStore.is_arrive == true) {
                    this.showAlarm = true
                    this.pollingEvent = PollingStore.pollingEvent;
                }
            } else {
                // console.log('error')
            }
        }, 5000);

    }
}
</script>

<style>
* {
    margin: 0;
    padding: 0;
}

html,
body {
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

.popup {
    position: fixed;
    width: 195px;
    bottom: 2px;
    left: 1px;
}

.popup .el-dialog__body {
    padding: 5px;
}
</style>