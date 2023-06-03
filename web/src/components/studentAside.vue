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


            <!-- <el-dialog title="闹钟提醒" :visible="showClassAlarm" :modal="false" :custom-class="'popup'"
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
            </el-dialog> -->
        </el-menu>
    </div>
</template>

<script>
import { TimeStore } from "@/store/time"
import { PollingStore } from "@/store/polling"
import { Notification } from 'element-ui';

export default {
    data() {
        return {
            speed: TimeStore.Tm, //倍速
            isCollapse: false,
            is_arrive: false,
            pollingCourse: {},
            pollingEvent: {},
            courseAlarm: {},
            eventAlarm: {},
            isCourseAlarmClosed: false,
            isEventAlarmClosed: false,
            letCourseOnce: false,
            letEventOnce: false,
        }
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
    },
    mounted() {
        // 轮询课程是否来临
        setInterval(async () => {
            const fg = await PollingStore.GetArrivedCourse(TimeStore.getTime());
            if (fg) {
                if (PollingStore.is_course_arrive == true) {
                    // this.showClassAlarm = true
                    this.pollingCourse = PollingStore.pollingCourse;
                    const alarmContent = '<div style="font-size: 17px">' + `<strong>${this.pollingCourse.course_name}</strong>` + '课程还有' + `<strong style="color: red">${1}</strong>` + '小时开始了!' + '</div>';
                    if (!this.isCourseAlarmClosed && !this.letCourseOnce) {
                        this.letCourseOnce = true
                        this.courseAlarm = Notification({
                            title: '课程闹钟！！！',
                            iconClass: 'el-icon-bell',
                            dangerouslyUseHTMLString: true,
                            message: alarmContent + '<br/><button style="display: inline-block; margin-top: 10px; margin-left: 55px; padding: 5px 5px; font-size: 14px; font-weight: bold; text-align: center; text-transform: uppercase; color: #fff; background-color: #007bff; border: none; border-radius: 5px; box-shadow: 0 2px 4px rgba(0, 0, 0, 0.2); cursor: pointer; transition: background-color 0.3s ease;">跳转至导航页面</button>',
                            onClick: () => {
                                // 点击后跳转到导航页面
                                this.$router.push('/studentMain/CourseNav');
                                console.log('Notification Clicked!')
                            },
                            onClose: () => {
                                this.isCourseAlarmClosed = true
                            },
                            duration: 0
                        })
                    }
                } else {
                    this.letCourseOnce = false
                    this.courseAlarm.close()
                    this.isCourseAlarmClosed = false
                }
            } else {
                console.log('error')
            }
        }, 3000);
        setInterval(async () => {
            const fg = await PollingStore.GetArrivedEvent(TimeStore.getTime());
            if (fg) {
                if (PollingStore.is_event_arrive == true) {
                    // this.showEventAlarm = true
                    this.pollingEvent = PollingStore.pollingEvent;
                    const alarmContent = '<div style="font-size: 17px">' + "距离" + `<strong>${this.pollingEvent.activity_name}</strong>` + '活动开始' + '只剩' + `<strong style="color: red">${this.pollingEvent.advance_mention_time}</strong>` + '分钟!' + '</div>';
                    if (!this.letEventOnce && !this.isEventAlarmClosed) {
                        this.letEventOnce = true
                        this.eventAlarm = Notification({
                            title: '活动闹钟！！！',
                            iconClass: 'el-icon-bell',
                            dangerouslyUseHTMLString: true,
                            message: alarmContent + '<br/><button style="display: inline-block; margin-top: 10px; margin-left: 55px; padding: 5px 5px; font-size: 14px; font-weight: bold; text-align: center; text-transform: uppercase; color: #fff; background-color: #007bff; border: none; border-radius: 5px; box-shadow: 0 2px 4px rgba(0, 0, 0, 0.2); cursor: pointer; transition: background-color 0.3s ease;">跳转至导航页面</button>',
                            onClick: () => {
                                // 点击后跳转到导航页面
                                this.$router.push('/studentMain/CourseNav');
                                console.log('Notification Clicked!')
                            },
                            onClose: () => {
                                this.isEventAlarmClosed = true
                            },
                            duration: 0
                        })
                    }   
                    
                } else {
                    this.letCourseOnce = false
                    this.eventAlarm.close()
                    this.isEventAlarmClosed = false
                }
            } else {
                console.log('error')
            }
        }, 3000);
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