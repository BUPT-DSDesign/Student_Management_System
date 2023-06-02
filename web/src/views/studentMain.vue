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
                    <el-dialog title="今日课程" :visible.sync="dialogVisible">
                        <el-timeline>
                            <el-timeline-item v-for="course in todayCourses" :key="course.id">
                                <div>{{ course.course_name }}</div>
                                <div>第{{ course.section_list[TimeStore.day ] }}节</div>
                            </el-timeline-item>
                        </el-timeline>
                    </el-dialog>
                    <!-- <el-dialog title="明天的活动" :visible.sync="dialogVisible">
                        <el-timeline>
                            <el-timeline-item v-for="(activity, index) in todayActivities" :key="index"
                                :timestamp="activity.timestamp" placement="top">
                                {{ activity.activity_name }}
                            </el-timeline-item>
                        </el-timeline>
                    </el-dialog> -->
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
            dialogVisible: true,
            myTimeStore: TimeStore,
        }
    },
    computed: {
        getRoutes() {
            return global.antRouter
        },
        computed: {
            todayCourses() {
                const { week, day } = this.myTimeStore;
                return this.courseList.filter(course => {
                    return course.week_schedule === week && course.section_list[day ];
                }).sort((a, b) => {
                    return a.section_list[day ] - b.section_list[day ];
                });
            }
        }
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
        }
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
            } else {
                console.log('获取用户课程失败')
            }
        }
        getTable();
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
</style>

