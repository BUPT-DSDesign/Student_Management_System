<template>
    <div class="wrapper">
        <div class="one item">
            <uploadAvatar ></uploadAvatar>
            <div class="user">
                <h1 style="color:black">{{ userInfo.username }}</h1>
            </div>
            <!-- <signature></signature> -->
            <el-card class="box-card">
                 <p id="hitokoto" class="signature"> 获取中...</p>

    </el-card>
           
        </div>
        <div class="two item">
            <div class="classblock" style="height: 120px;float: left;">
                <img src="../assets/image/class.png" style="height: 120px;width: 120px; margin-left:11px">
                <span class="right-class">今天共有<h6 style="font-size:25px"> {{ classNumber_remaining }}</h6>节课</span>
            </div>
            <div class="eventblock" style="height: 120px;float: left;">
                <img src="../assets/image/event.png" style="height: 120px;width: 120px; margin-left:11px">
                <span class="right-event">今天共有<h6 style="font-size:25px">{{ eventNumber_remaining }}</h6>个活动</span>
            </div>
        </div>
        <div class="three item">
            <div class="ProgressBar">
                <h6>本日进度</h6>
                <el-progress :percentage="60" :stroke-width="10"></el-progress>
                <h6>本周进度</h6>
                <el-progress :percentage="80" :stroke-width="10"></el-progress>
                <h6>本学期进度</h6>
                <el-progress :percentage="20" :stroke-width="10"></el-progress>
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
import { useUserStore } from '@/pinia/modules/user';
import { useCourseStore } from '@/pinia/modules/course';
import { calcurWeek } from "@/utils/time"

fetch('https://v1.hitokoto.cn', { c: 'd', min_length: 5, max_length: 15})
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

export default {

    beforeMount() {
        // 在个人主页渲染的时候, 应该向后端请求个人信息
        const getUserInfo = async () => {
            const fg = await this.useUserStore.GetUserInfo()
            if (fg) {
                this.userInfo = this.useUserStore.userInfo
                console.log(this.userInfo)
            } else {
                console.log('获取用户信息失败')
            }
        }
        getUserInfo();
        const getTable = async () => {
            const fg = await this.useCourseStore.GetCourseTable();
            if (fg) {
                this.courseList = this.useCourseStore.rdata.course_list;
                //根据当前周，查找在本周的课程
                this.courseList = this.courseList.filter((item) => {
                    return item.week_schedule.indexOf(calcurWeek().week) != -1;
                });
                //查找本天的课程，然后将他们按照顺序排列。
                for (let i = 0; i < this.courseList.length; i++) {
                    for (let j = 0; j < this.courseList[i].section_list.length; j++) {
                        if (this.courseList[i].section_list[j] / 9 < 1) {
                            this.curcourseList.push({
                                content: this.courseList[i].course_name,
                                timestamp: '第' + this.courseList[i].section_list[j] % 9 + '节',
                                size: 'large',
                                type: 'primary',
                                color: '#8ce99a',

                            })
                        }
                        if (this.courseList[i].section_list[j] / 9 == 1) {
                            this.curcourseList.push({
                                content: this.courseList[i].course_name,
                                timestamp: '第9节',
                                size: 'large',
                                type: 'primary',
                                color: '#8ce99a',
                            })
                        }
                    }
                }
            } else {
                console.log('获取用户课程失败')
            }
        }
        getTable();
    },
    data() {
        return {
            classNumber_remaining: 3,
            eventNumber_remaining: 2,
            useUserStore: new useUserStore(),
            useCourseStore: new useCourseStore(),
            userInfo: {},
            courseList: [],
            curcourseList: [],
        }
    },
    components: {
        uploadAvatar,
        signature,
    },
    methods: {
        preventclick() {
            
        }
    }
}
</script>

<style>
.signature{
    font-size:12px;
}
.el-card__body {
    padding: 17px;
}
.box-card {
    margin:15px;
    width: 300px;
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

.user {
    margin-top: 50px;
    margin-right: 50px;
    float: right;
}

html {
    background-color: #e9ecef;
}



h6 {
    color: black;
    font-size: 15px;
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
}
</style>

