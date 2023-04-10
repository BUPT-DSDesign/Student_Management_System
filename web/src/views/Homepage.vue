<template>
    <div class="wrapper">
        <div class="one item">
            <div class="user-container">
                <uploadAvatar :userInfo="userInfo"></uploadAvatar>
                <div class="user-info">
                    <h5 style="color:black; font-size: medium; text-align: left;">用户名: {{ userInfo.username }}</h5>
                    <h5 style="color:black; font-size: medium; text-align: left;">学号: {{ userInfo.student_id }}</h5>
                </div>
            </div>
            
            <signature :userInfo="userInfo"></signature>
            
           
            <el-card class="box-card">
                <p id="hitokoto" class="yiyan"> 获取中...</p>
            </el-card>

        </div>
        <div class="two item">
            <div class="classblock" style="height: 120px;float: left;">
                <img src="../assets/image/class.png" style="height: 120px;width: 120px; margin-left:11px">
                <span class="right-class">今天共有<h6 style="font-size:25px"> {{ curcourseList.length }}</h6>节课</span>
            </div>
            <div class="eventblock" style="height: 120px;float: left;">
                <img src="../assets/image/event.png" style="height: 120px;width: 120px; margin-left:11px">
                <span class="right-event">今天共有<h6 style="font-size:25px">{{ eventNumber_remaining }}</h6>个活动</span>
            </div>
        </div>
        <div class="three item">
            <div class="ProgressBar">
                <h6>本日进度</h6>
                <el-progress :percentage="calDaypercent()" :stroke-width="10"></el-progress>
                <h6>本周进度</h6>
                <el-progress :percentage="calWeekpercent()" :stroke-width="10"></el-progress>
                <h6>本学期进度</h6>
                <el-progress :percentage="calTermpercent()" :stroke-width="10"></el-progress>
            </div>
        </div>
        <div class="four item">
            <h2 style="color:black;margin-top:20px;margin-left: 62px">今日课程</h2>
            <div class="block">
                <el-timeline>
                    <el-timeline-item v-for="(activity, index) in curcourseList" :key="index" :type="activity.type"
                        :color="activity.color" :size="activity.size" :timestamp="activity.timestamp">
                        {{ activity.content }}
                    </el-timeline-item>
                </el-timeline>
            </div>
        </div>
    </div>
</template>
<script>
import uploadAvatar from '@/components/Homepage/uploadAvatar.vue';
import signature from '@/components/Homepage/signature.vue';
import { CourseStore } from '@/store/course';
import { UserStore } from '@/store/user'
import { TimeStore } from '@/store/time';
export default {
    data() {
        return {
            eventNumber_remaining: 0,
            userInfo: {},
            courseList: [],
            curcourseList: []
        }
    },
    created() {
        // 在个人主页渲染的时候, 应该向后端请求个人信息
        const getUserInfo = async () => {
            const fg = await UserStore.GetUserInfo()
            if (fg) {
                // console.log(UserStore)
                this.userInfo = UserStore.userInfo
            } else {
                console.log('获取用户信息失败')
            }
        }
        getUserInfo()
        
        const getTable = async () => {
            const fg = await CourseStore.GetCourseTable();
            if (fg) {
                this.courseList = CourseStore.courseList;
                //根据当前周，查找在本周的课程
                this.courseList = this.courseList.filter((item) => {
                    return item.week_schedule.indexOf(TimeStore.week) != -1;
                });
                //查找本天的课程，然后将他们按照顺序排列。
                for (let i = 0; i < this.courseList.length; i++) {
                    for (let j = 0; j < this.courseList[i].section_list.length; j++) {
                        if (this.courseList[i].section_list[j] / 9 < TimeStore.day) {
                            this.curcourseList.push({
                                content: this.courseList[i].course_name,
                                timestamp: this.courseList[i].section_list[j] % 9,
                                size: 'large',
                                type: 'primary',
                                color: '#8ce99a',
                            })
                        }
                        if (this.courseList[i].section_list[j] / 9 == 1) {
                            this.curcourseList.push({
                                content: this.courseList[i].course_name,
                                timestamp: 9,
                                size: 'large',
                                type: 'primary',
                                color: '#8ce99a',
                            })
                        }
                    }
                }
                //对课程按照节次进行排序
                this.curcourseList.sort(function (a, b) {
                    return a.timestamp - b.timestamp;
                });
                //添加上汉字
                this.curcourseList = this.curcourseList.map(function (item, index, arr) {
                    item.timestamp = '第' + item.timestamp + '节';
                    return item;
                })
            } else {
                console.log('获取用户课程失败')
            }
        }
        getTable()
        fetch('https://v1.hitokoto.cn')
            .then(function (res) {
                return res.json();
            })
            .then(function (data) {
                var hitokoto = document.getElementById('hitokoto');
                hitokoto.innerText = data.hitokoto;
            })
            .catch(function (err) {
                console.error(err);
            })
    },
    components: {
        uploadAvatar,
        signature
    },
    computed: {
        calDaypercent() {
            return function () {
                return Math.ceil((parseInt(TimeStore.minute) * 60 + parseInt(TimeStore.hour) * 3600) / 864);
            }
        },
        calWeekpercent() {
            return function () {
                return Math.ceil((parseInt(TimeStore.day) * 100) / 7);
            }
        },
        calTermpercent() {
            return function () {
                return Math.ceil((parseInt(TimeStore.week) * 100) / 16);
            }
        },
    }
}
</script>

<style>
.user-container {
    overflow: hidden;
    width: 100%;
}
.user-info {
    width: 40%;
    float: right;
    padding-top: 20px;
}
.yiyan {
    font-size: 18px;
    color: #2177b8;
    width: 100%;
}

.el-card.is-always-shadow, .el-card.is-hover-shadow:focus, .el-card.is-hover-shadow:hover {
    box-shadow: 0 0;
}

.el-card__body {
    padding: 5px;
}

.box-card {
    width: 100%;
    position: absolute;
    bottom: 0;
    right: 0;
    border-block-width: 0px;
    margin: 0;
    background-color: #f8f9fa;
}

.block {
    margin: 30px 70px;
}

.wrapper {
    display: grid;
    grid-template-columns: repeat(3, 1fr);
    grid-gap: 20px;
    grid-auto-rows: minmax(150px, auto);
}

.one {
    grid-column: 1 / 2;
    grid-row: 1 / 3;
    background: #f8f9fa;
    position: relative;
}

.two {
    grid-column: 2 / 4;
    grid-row: 1 / 2;
    background: #f8f9fa;
}

.three {
    grid-column: 1 / 2;
    grid-row: 3 / 5;
    background: #f8f9fa;
}

.four {
    grid-column: 2 / 4;
    grid-row: 2 / 5;
    background: #f8f9fa;
    
}

.item {
    text-align: center;
    font-size: 200%;
    color: #fff;
}

html {
    background-color: #e9ecef;
}

h6 {
    color: black;
    font-size: 15px;
}

h1 {
    font-size: 30px;
    margin-left: 5px;
    margin-right: -25px;
    margin-top: 20px;
}

.ProgressBar {
    margin-top: 30px;
}

.right-event,
.right-class {
    color: black;
    font-size: 16px;
    float: right;
    margin-top: 40px;
    margin-right: 40px;
    margin-left: 20px;
}

.el-progress {
    margin-top: 15px;
}

.classblock,
.eventblock {
    background-color: #e9ecef;
    margin: 10px 47px;
}</style>
