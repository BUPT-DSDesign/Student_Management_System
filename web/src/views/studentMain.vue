<template>
    <div>
        <el-container>
            <el-aside width="200px">
                <!-- 侧边栏 -->
                <studentAside :routes="getRoutes"></studentAside>
            </el-aside>
            <el-container>
                <el-header>
                    <!-- 上方 -->
                    <Header></Header>
                </el-header>
                <el-main>
                    <!-- 子路由的路由出口 -->
                    <router-view></router-view>
                    <el-dialog title="明日课程、活动安排" :visible.sync="dialogVisible">
                        <div class="timeline-container" style="float:left">
                            <div class="timeline-title">明日课程</div>
                            <el-timeline>
                                <el-timeline-item v-for="(course, index) in curcourseList" :key="index" :type="course.type"
                                    :timestamp="course.timestamp" color="#8ce99a">
                                    {{ course.content }}
                                </el-timeline-item>
                            </el-timeline>
                        </div>
                        <div class="timeline-container" style="float:right">
                            <div class="timeline-title">明日活动</div>
                            <el-timeline>
                                <el-timeline-item v-for="(activity, index) in todayActivities" :key="index"
                                    :timestamp="activity.timestamp" color="#8ce99a">
                                    {{ activity.activity_name }}
                                </el-timeline-item>
                            </el-timeline>
                        </div>
                        <div style="clear:both;"></div> <!-- 添加清除浮动的元素 -->
                    </el-dialog>
                </el-main>
            </el-container>
        </el-container>
    </div>
</template>
<script>
import studentAside from '../components/studentAside.vue'
import Header from '../components/Header.vue'
import { CourseStore } from '@/store/course';
import { EventStore } from '@/store/event';
import { TimeStore } from '@/store/time';


export default {
    data() {
        return {
            userRole: 'Topest',
            eventList: [],
            todayActivities: [],
            courseList: [],
            curcourseList: [],
            dialogVisible: false,
            myTimeStore: TimeStore,
        }
    },
    computed: {
        getRoutes() {
            return global.antRouter
        },
    },
    methods: {
        getTodayActivities() {
            const today = TimeStore.day;
            const thisWeek = `第${TimeStore.week}周`;
            const todayActivities = [];
            this.eventList.forEach(activity => {
                const { start_time, frequency, activity_name } = activity;
                if (start_time.includes(thisWeek) && start_time.includes(`星期${today}`)) {
                    const hour = parseInt(start_time.split("-")[2].split(":")[0]);
                    if (frequency === 0 && TimeStore.hour >= hour) {
                        todayActivities.push({
                            activity_name,
                            time: `${hour}:00`,
                            timestamp: new Date(
                                TimeStore.hour >= hour
                                    ? Date.now()
                                    : Date.now() - 24 * 60 * 60 * 1000
                            )
                        });
                    } else if (frequency === 1) {
                        todayActivities.push({
                            activity_name,
                            timestamp: `${hour}:00`
                        });
                    } else if (frequency === 2 && start_time.includes(`星期${today}`)) {
                        if (TimeStore.hour >= hour) {
                            todayActivities.push({
                                activity_name,
                                timestamp: `${hour}:00`
                            });
                        } else {
                            todayActivities.push({
                                activity_name,
                                timestamp: `${hour}:00`
                            });
                        }
                    }
                }
            });
            todayActivities.sort((a, b) => a.timestamp - b.timestamp);
            return todayActivities;
        },

    },

    watch: {
        myTimeStore: {
            handler(val) {
                if (val.hour == 20 && val.minute == 0) {
                    this.dialogVisible = true;
                }
            },
            deep: true
        },

    },
    components: {
        studentAside,
        Header
    },
    created() {
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

                        if (this.courseList[i].section_list[j] >= (TimeStore.day - 1) * 9 + 1
                            && this.courseList[i].section_list[j] <= TimeStore.day * 9) {
                            this.curcourseList.push({
                                content: this.courseList[i].course_name,
                                timestamp: (this.courseList[i].section_list[j] % 9),
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
        //获取活动
        const getActivityTable = async () => {
            const fg = await EventStore.GetEventTable()
            if (fg) {
                this.eventList = EventStore.eventList;
                this.todayActivities = this.getTodayActivities();
            } else {
                console.log('error')
            }
        }
        getActivityTable()

    }
}
</script>
<style>
.el-header {
    padding: 0;
}

.el-main {
    padding: 0;
}

.timeline-container {
    margin:20px 100px;
}

.timeline-title {
    font-size: 16px;
    font-weight: bold;
    margin-left: 20px;
    margin-top: -40px;
    margin-bottom: 30px;
}
</style>

