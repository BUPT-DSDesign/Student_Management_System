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
                    <el-dialog title="明天要上的课程" :visible.sync="dialogVisible" width="30%" >
                        <el-timeline>
                            <el-timeline-item v-for="(activity, index) in curcourseList" :key="index" :type="activity.type"
                                :color="activity.color" :size="activity.size" :timestamp="activity.timestamp">
                                {{ activity.content }}
                            </el-timeline-item>
                        </el-timeline>
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
import { TimeStore } from '@/store/time';

export default {
    computed: {
        getRoutes() {
            return global.antRouter
        }
    },
    data() {
        return {
            userRole: 'Topest',
            curcourseList: [],
            dialogVisible: false,
            myTimeStore:TimeStore,
        }
    },
    watch: {
        myTimeStore: {
            handler(val) {
                if (val.hour == 20&&val.minute==0) {
                    this.dialogVisible = true;
                }
            },
            deep: true
        }
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
                //查找明天的课程，然后将他们按照顺序排列。
                for (let i = 0; i < this.courseList.length; i++) {
                    for (let j = 0; j < this.courseList[i].section_list.length; j++) {
                        if (this.courseList[i].section_list[j] / 9 < (TimeStore.day+1)%7) {
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

