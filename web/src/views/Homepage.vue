<template>
    <div class="wrapper">
        <div class="one item">
            <div class="user-container">
                <uploadAvatar :userInfo="avatarUrl"></uploadAvatar>
                <div class="user-info">
                    <h4>姓名:{{ userInfo.username }}</h4>
                    <h4>学号:{{ userInfo.student_id }}</h4>
                    <h4>班级:{{ 2021211309 }}</h4>
                </div>
            </div>
            <signature :userInfo="userInfo"></signature>
            <el-card class="box-card">
                <p id="hitokoto" class="yiyan"> 获取中...</p>
            </el-card>

        </div>
        <div class="two item">
            <div class="classblock" style="height: 80%">
                <img src="../assets/image/class.png" style="height: 100%;width: auto; margin-left:11px">
                <span class="right-class">今天共有<h6 style="font-size:25px"> {{ curcourseList.length }}</h6>节课</span>
            </div>
            <div class="eventblock" style="height: 80%">
                <img src="../assets/image/event.png" style="height: 100%;width: auto; margin-left:11px">
                <span class="right-event">今天共有<h6 style="font-size:25px">{{ eventCount }}</h6>个活动</span>
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
        <div class="four item" style="overflow-y: scroll">
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
import { EventStore } from '@/store/event';
import { UserStore } from '@/store/user'
import { TimeStore } from '@/store/time';


function getTodayActivityNum(activityList, currentTime) {
  const today = currentTime.day;
  const thisWeek = `第${currentTime.week}周`;
  let count = 0;
  activityList.forEach(activity => {
    const { start_time, frequency } = activity;
    console.log(start_time, frequency)
    if (start_time.includes(thisWeek) && start_time.includes(`星期${today}`)) {
      if (frequency === 0) {
        count++
        // const startTime = start_time.split('-')[2];
        // if (currentTime.hour > parseInt(startTime.split(':')[0]) || (currentTime.hour === parseInt(startTime.split(':')[0]) && currentTime.minute >= parseInt(startTime.split(':')[1]))) {
        //   count++;
        // }
      } else if (frequency === 1) {
        count++;
      } else if (frequency === 2 && start_time.includes(`星期${today}`)) {
        const startTime = start_time.split('-')[2];
        if (currentTime.hour > parseInt(startTime.split(':')[0]) || (currentTime.hour === parseInt(startTime.split(':')[0]) && currentTime.minute >= parseInt(startTime.split(':')[1]))) {
          count++;
        }
      }
    }
  });
  return count;
}
export default {
    data() {
        return {
            eventCount: 0,
            userInfo: {},
            eventInfo:{},
            courseList: [],
            curcourseList: [],
            avatarUrl: '',
        }
    },
    created() {
        const userId = window.localStorage.getItem('userId')
        // console.log(userId)
        this.avatarUrl = `http://127.0.0.1:8080/static/${userId}.jpg`
        // 在个人主页渲染的时候, 应该向后端请求个人信息
        const getUserInfo = async () => {
            const fg = await UserStore.GetUserInfo()
            if (fg) {
    
                this.userInfo = UserStore.userInfo
            } else {
                console.log('获取用户信息失败')
            }
        }
        getUserInfo()
        // 在个人主页渲染的时候, 应该向后端请求个人信息
        const getEventInfo = async () => {
            const fg = await EventStore.GetEventTable()
            if (fg) {
                this.eventInfo = EventStore.eventList;
                this.eventCount = getTodayActivityNum(this.eventInfo, TimeStore);
               
            } else {
                console.log('获取活动信息失败')
            }
        }
        getEventInfo();

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
                        //通过获取this.courseList[i].section_list[j] 和TimeStore.day对比，发现是今天的课程
                        if (this.courseList[i].section_list[j] >= (TimeStore.day - 1) * 9 + 1
                            && this.courseList[i].section_list[j] <= TimeStore.day * 9) {
                            this.curcourseList.push({
                                content: this.courseList[i].course_name,
                                timestamp: this.courseList[i].section_list[j] % 9==0?9: (this.courseList[i].section_list[j] % 9),
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
    width: 50%;
    float: right;

    padding-top: 10px;
}
h4 {
    color:black; 
    font-size: medium; 
    text-align: center;
    display: inline-block;
    height: 33.3%;
}
.yiyan {
    font-size: 15px;
    color: #2177b8;
    width: 100%;
}

.el-card.is-always-shadow, .el-card.is-hover-shadow:focus, .el-card.is-hover-shadow:hover {
    box-shadow: 0 0;
}

.box-card {
    width: 100%;
    position: absolute;
    bottom: 0;
    right: 0;
    border-block-width: 0px;
    margin: 0;
    padding: 0;
    background-color: #f8f9fa;
}

.el-card__body {
    padding: 0px;
    padding-bottom: 1px;
}

.block {
    margin: 30px 70px;
}

.wrapper {
    display: grid;
    grid-template-columns: repeat(3, 1fr);
    grid-gap: 20px;
    grid-auto-rows: 20vh;
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
    display: flex;
    justify-content: space-around;
    align-items: center;
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
    margin-top: 10px;
    margin-right: 40px;
    margin-left: 20px;
}

.el-progress {
    margin-top: 15px;
}

.classblock,
.eventblock {
    background-color: #e9ecef;
}</style>
