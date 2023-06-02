<template>
    <!-- 左侧：课表，切换功能，点击弹窗功能 -->
    <div class='class-table'>
        <div class='table-wrapper'>
            <div class='tabel-container'>
                <table>
                    <thead>
                        <tr>
                            <th>时间</th>
                            <th v-for='(week, index) in weeks' :key="index"> {{ week }}</th>
                        </tr>
                    </thead>
                    <tbody>
                        <tr v-for='(classtime, classIndex) in time' :key='classIndex'>
                            <td>
                                <p>{{ '第' + (classIndex + 1) + '节' }}</p>
                                <p class='period'>{{ classtime }}</p>
                            </td>
                            <!-- 填写课程信息  -->
                            <td v-for='weekIndex in 7' :key='weekIndex' @click="clickClass($event)">
                                {{ filltable(classIndex, weekIndex - 1) }}
                            </td>
                        </tr>
                    </tbody>
                </table>
            </div>
        </div>
        <el-tag effect="dark" style="position: absolute; bottom: 0;">第{{ curWeek }}周</el-tag>
        <el-button-group>
            <el-button type="primary" icon="el-icon-arrow-left" @click="toLastweek()" style="position: absolute; bottom: 0;left:407px">上一周</el-button>
            <el-button type="primary" @click="toNextweek()" style="position: absolute; bottom: 0;left: 518px">下一周<i
                    class="el-icon-arrow-right el-icon--right"></i></el-button>
        </el-button-group>

        <!-- 点击弹窗 -->
        <el-dialog title="课程详情" :visible.sync="dialogVisible1" width="30%" :before-close="handleClose">
            <span>{{ courseInfo.course_name }}</span>
            <div style="padding: 20px">
                <p><strong>上课地点：</strong>{{ courseInfo.classroom }}</p>
                <p><strong>授课老师：</strong>{{ courseInfo.teacher }}</p>
                <p><strong>上课时间：</strong>{{ courseInfo.class_time }}</p>
                <p><strong>上课周次：</strong>{{ courseInfo.week_schedule }}</p>
                <p><strong>课程性质：</strong>{{ courseInfo.is_compulsory == true ? '必修' : '选修' }}</p>
                <p><strong>考试时间：</strong>{{ courseInfo.exam_time }}</p>
                <p><strong>考试地点：</strong>{{ courseInfo.exam_location }}</p>
            </div>
            <!-- <div class="course_name">课程名称：{{ curClassData.course_name }}</div>
                    <div class="course_time">课程时间：{{ curClassData.section_list }}</div>
                    <div class="course_address">课程地点：{{ curClassData.classroom }}</div> -->
            <span slot="footer" class="dialog-footer">
                <el-button @click="dialogVisible1 = false">取 消</el-button>
                <el-button type="primary" @click="dialogVisible1 = false">确 定</el-button>
            </span>
        </el-dialog>
        <!-- 搜索弹窗 -->
        <el-dialog title="课程详情" :visible.sync="dialogVisible2" width="30%" :before-close="handleClose">
            <div v-for="(item, index) in searchlist" :key="index">
                <div class="course_name">课程名称：{{ item.course_name }}</div>
                <div class="course_time">课程时间：{{ item.section_list }}</div>
                <div class="course_address">课程地点：{{ item.classroom }}</div>
            </div>
            <span slot="footer" class="dialog-footer">
                <el-button @click="dialogVisible2 = false">取 消</el-button>
                <el-button type="primary" @click="dialogVisible2 = false">确 定</el-button>
            </span>
        </el-dialog>
        <!-- 右侧：课程搜索功能，通过选择课程名称，时间，地点，对关键字进行模糊匹配，查询后直接在下方显示 -->
        <el-row :gutter="12">
            <el-col :span="100">
                <el-card shadow="always" class="searchbox">
                    <h2 style="text-align: center;">关键字选择</h2>
                    <el-radio-group v-model="radio" style="padding-left: 70px;">
                        <el-radio :label="1">课程名称</el-radio>
                        <el-radio :label="2">课程地点</el-radio>
                    </el-radio-group>
                    <h2 style="text-align: center; padding-top: 20px;">查询课程</h2>
                    <el-form>
                        <el-input size="small" v-model="keyWord" placeholder="请输入查询关键词"
                            prefix-icon="el-icon-search"></el-input>
                    </el-form>
                    <ul class="list-group">
                        <li v-for="(p, index) of filclasslist" :key="index" @click="chooseclass($event)"
                            style="font-size:10px;">
                            {{ radio == 1 ? p.course_name : p.classroom }}
                        </li>
                    </ul>
                    <div class="buttons-combine">
                        <el-button type="info" class="searchBtn" @click="clearInput">清空</el-button>
                        <el-button type="primary" class="searchBtn" @click="onSubmit">查询</el-button>
                    </div>

                    <div class="select-class" style="color: red">
                        -------------<el-button type="danger"
                            @click="openSelectCourseSystem">进入选课系统</el-button>-------------
                    </div>
                </el-card>
            </el-col>
        </el-row>
        <!-- 选课系统 -->
        <el-dialog title="选课系统" :visible.sync="selectCourseSystemVis" width="50%">
            <h3 style="color:black">
                选修课列表
            </h3>
            <el-table :data="selectiveCourseData" height="260" border style="width: 100%">
                <el-table-column label="课程名" fixed align="center">
                    <template slot-scope="scope">
                        <span>{{ scope.row.course_name }}</span>
                    </template>
                </el-table-column>

                <el-table-column label="教师" fixed align="center">
                    <template slot-scope="scope">
                        <span>{{ scope.row.teacher }}</span>
                    </template>
                </el-table-column>

                <el-table-column label="操作" fixed align="center" width="300">
                    <template slot-scope="scope">
                        <el-button size="small" type="primary" @click="seeCourseInfo(scope.row)">查看课程详情</el-button>
                        <!-- 点击弹窗 -->

                        <el-button size="small" type="success" :disabled="scope.row.is_selected"
                            @click="openConfirmFrame(scope.row)">{{ scope.row.is_selected == true ? '已选' : '选课' }}</el-button>
                    </template>
                </el-table-column>
            </el-table>

            <span slot="footer" class="dialog-footer">
                <el-button @click="selectCourseSystemVis = false">关闭</el-button>
            </span>
        </el-dialog>
        <el-dialog title="课程详情" :visible.sync="courseInfoVis" width="30%">
            <div>课程名称：{{ selectedCourseInfo.course_name }}</div>
            <div>课程节次：{{ selectedCourseInfo.section_list }}</div>
            <div>课程周次：{{ selectedCourseInfo.week_schedule }}</div>
            <div>课程地点：{{ selectedCourseInfo.classroom }}</div>
            <div>教师：{{ selectedCourseInfo.teacher }}</div>
            <div>联系方式：{{ selectedCourseInfo.contact }}</div>
            <div>考试时间：{{ selectedCourseInfo.exam_time }}</div>
            <div>考试地点：{{ selectedCourseInfo.exam_location }}</div>
            <span slot="footer" class="dialog-footer">
                <el-button @click="courseInfoVis = false">取 消</el-button>
                <el-button type="primary" @click="courseInfoVis = false">确 定</el-button>
            </span>
        </el-dialog>
    </div>
</template>
<script>
import { Message } from 'element-ui'
import { CourseStore } from '@/store/course';
import { LogStore } from '@/store/log';
import { TimeStore } from '@/store/time';
export default {
    data() {
        return {
            curClass: '', //记录当前点击单元格课程的名称
            curClassData: {},  //记录当前点击单元格课程的信息（对象类型
            curWeek: 1,
            weeks: ['周一', '周二', '周三', '周四', '周五', '周六', '周日'],
            time: ['08:00-09:00', '09:00-10:00', '10:00-11:00', '11:00-12:00', '13:00-14:00', '14:00-15:00', '15:00-16:00', '16:00-17:00', '17:00-18:00'],
            classData: [],
            courseInfo: [],
            dialogVisible1: false, //点击弹窗的可见性
            dialogVisible2: false, //查询弹窗的可见性
            radio: 1,//多选框默认选中的单元
            keyWord: '', //用户查询的课程关键字
            filclasslist: [], //模糊匹配后的课程列表（li中的填充对象
            searchlist: [],//搜索后返回的数组对象(查询弹窗中的填充对象)
            selectCourseSystemVis: false, //选课系统弹窗的可见性
            selectiveCourseData: [],
            selectedCourseInfo: {},
            courseInfoVis: false
        };
    },
    created() {
        const getCourseTable = async () => {
            const fg = await CourseStore.GetCourseTable()
            if (fg) {
                this.classData = CourseStore.courseList
                console.log(this.classData)
            } else {
                console.log('error')
            }
        }
        getCourseTable()

        const getSelectiveCourse = async () => {
            const fg = await CourseStore.GetSelectiveCourse()
            if (fg) {
                this.selectiveCourseData = CourseStore.selectiveCourseList
            } else {
                console.log('error')
            }
        }
        getSelectiveCourse()
    },
    watch: {
        keyWord(newvalue) {
            //如果当前关键词为空
            if (newvalue === "") {
                this.filclasslist = [];
            }
            else {
                if (this.radio == 1) {
                    //过滤
                    this.filclasslist = this.classData.filter((item) => {
                        return item.course_name.indexOf(newvalue) != -1;
                    });
                }

                else if (this.radio == 2) {
                    this.filclasslist = this.classData.filter((item) => {
                        return item.classroom.indexOf(newvalue) != -1;
                    });
                    //去重
                    this.filclasslist = this.dedup(this.filclasslist);
                }
            }
        },
    },
    methods: {
        clearInput() {
            this.keyWord = ''
        },
        seeCourseInfo(data) {
            this.selectedCourseInfo = data
            this.courseInfoVis = true
        },
        openConfirmFrame(course) {
            this.$confirm('确定选修该课程吗？', '提示', {
                confirmButtonText: '确定',
                cancelButtonText: '取消',
                type: 'warning'
            }).then(async () => {
                const fg = await CourseStore.SelectCourse(course.course_id)
                if (fg) {
                    this.$message({
                        type: 'success',
                        message: '选修课程成功!'
                    });
                    location.reload();
                    // 创建一个日志对象
                    const log = {
                        "create_time": TimeStore.getTime(),
                        "content": "选修课程成功, 课程名为：" + course.course_name,
                    }
                    console.log(log)
                    LogStore.AddLog(log)

                } else {
                    this.$message({
                        type: 'error',
                        message: '选修失败!'
                    });
                }
            }).catch(() => {
                this.$message({
                    type: 'info',
                    message: '已取消选修'
                });
            });
        },

        // 打开选课系统
        openSelectCourseSystem() {
            this.selectCourseSystemVis = true
        },
        //将相同教室的数组元素去重
        dedup(arr) {
            const res = new Map()
            return arr.filter((item) => !res.has(item.classroom) && res.set(item.classroom, 1))
        },
        //点击下一周
        toNextweek() {
            if (this.curWeek >= 19) {
                return;
            }
            this.curWeek++;
            this.curCell = 0;//重置单元格计数器
            this.$forceUpdate(); //重新刷新页面
        },
        //点击上一周
        toLastweek() {
            if (this.curWeek <= 1) {
                return;
            }
            this.curWeek--;
            this.curCell = 0;//重置单元格计数器
            this.$forceUpdate(); //重新刷新页面
        },
        //点击课程单元格
        clickClass(event) {
            if (event.target.innerText == '') {
                Message.info({
                    showClose: true,
                    center: true,
                    message: '当前课节无课',
                    duration: 1000,
                })
                return;
            }
            //更改当前点击课程的名称
            this.curClass = event.target.innerText;
            // 循环遍历找到数据库中该课程的所有信息
            for (let i = 0; i < this.classData.length; i++) {
                if (this.classData[i].course_name == this.curClass) {
                    console.log('相等')
                    //如果有该课程，更新curClassData
                    this.curClassData = this.classData[i];
                }
            }
            //点击弹窗
            this.dialogVisible1 = true;
            this.courseInfo = this.processCourseInfo(this.curClassData);
        },
        // 关闭弹窗
        handleClose(done) {
            done();
        },
       getCourseTime(sectionList) {
            const weekdays = ['周一', '周二', '周三', '周四', '周五', '周六', '周日'];
            const sections = ['第一节', '第二节', '第三节', '第四节', '第五节', '第六节', '第七节', '第八节', '第九节'];
            const courseTime = [];
            let currentCourse = {
                weekday: '',
                sections: []
           };
            sectionList.sort();
            sectionList.forEach(section => {
                const weekday = weekdays[Math.floor((section - 1) / 9)];
                const sectionIndex = (section - 1) % 9;
                let sectionName = sections[sectionIndex];
                if (weekday === currentCourse.weekday && sectionIndex === currentCourse.sections[currentCourse.sections.length - 1] + 1) {
                    currentCourse.sections.push(sectionIndex);
                } else {
                    if (currentCourse.weekday) {
                        courseTime.push(currentCourse);
                    }
                    currentCourse = {
                        weekday,
                        sections: [sectionIndex]
                    };
                }
            });
            if (currentCourse.weekday) {
                courseTime.push(currentCourse);
            }
            const formattedTime = courseTime.map(course => {
                const { weekday, sections } = course;
                if (sections.length === 1) {
                    return `${weekday}第${sections[0] === 0 ? sections[0] + 1 : `第${sections[0] + 1}节`}节`;
                } else if (sections.length === 9) {
                    return `${weekday}全天`;
                } else {
                    return `${weekday}第${sections[0] + 1}~${sections[sections.length - 1] + 1}节`;
                }
            });
            return formattedTime.join('，');
        },
        processCourseInfo(course) {
            const days = ['周一', '周二', '周三', '周四', '周五', '周六', '周日'];
            const sections = ['第一节', '第二节', '第三节', '第四节', '第五节', '第六节', '第七节', '第八节', '第九节'];
            const classTime = this.getCourseTime(course.section_list)

            return {
                course_name: course.course_name,
                classroom: course.classroom,
                teacher: course.teacher,
                class_time: classTime,
                week_schedule: "第" + course.week_schedule + "周",
                exam_time: course.exam_time,
                exam_location: course.exam_location,
                is_compulsory: course.is_compulsory
            };
        },
        //查询的点击事件
        onSubmit() {
            let search = {};
            if (this.radio == 1) {
                search.is_course_name = 1;
                search.key_word = this.keyWord;
            }
            else if (this.radio == 2) {
                search.is_course_name = 0;
                search.key_word = this.keyWord;
            }
            if (search.key_word == '') {
                this.$message({
                    showClose: true,
                    center: true,
                    message: '不能输入空的关键词',
                    type: 'error'
                });
                return;
            }
            const inqueryCourse = async (data) => {
                console.log('测试查询')
                console.log(data)

                const fg = await CourseStore.inquiryCourseInfo(data);
                if (fg) {
                    this.searchlist = CourseStore.searchCourseList;
                    this.$message({
                        showClose: true,
                        center: true,
                        message: '查询课程成功',
                        type: 'success'
                    });
                    search = {}
                } else {
                    this.$message({
                        showClose: true,
                        center: true,
                        message: '课程失败',
                        type: 'error'
                    });
                    search = {}
                }
            }
            inqueryCourse(search);
            //点击弹窗
            this.dialogVisible2 = true;
        },
        //点击ul自动填充input
        chooseclass(e) {
            this.keyWord = e.target.innerText;
        },
    },
    computed: {
        //填充单元格
        filltable() {
            return function (classIndex, weekIndex) {
                // 因为现在数据中只有两种课程，所以这里i < 2
                for (let i = 0; i < this.classData.length; i++) {
                    //如果是周数组中有当前周
                    if (this.classData[i].week_schedule.indexOf(this.curWeek) != -1) {
                        //如果是节数组中有当前节
                        if (this.classData[i].section_list.indexOf(weekIndex * 9 + classIndex + 1) != -1) {
                            return this.classData[i].course_name;
                        }
                    }
                }
                return '';
            }

        },
    }
};

</script>

<style lang='scss' scoped>
.select-class {
    text-align: center;
    margin-top: 50px;

}

.buttons-combine {
    display: flex;
    justify-content: space-evenly;
}

.list-group {
    color: #adb5bd;
    margin-left: 10px;
    margin-top: 10px;
}

ul {
    margin-top: 20px;
}

.searchBtn {
    margin-top: 10px;
}

h2 {
    margin: 18px;
    margin-left: 10px;
    font-size: 20px;
}

.searchbox {
    float: right;
    padding: 20px;
    width: 350px;
    height: 400px;
    margin-top: -500px;
    margin-left: 650px;
}

.el-form {
    height: 37px;
    background-color: #fff;
}

.el-tag {
    font-size: 18px;
    height: 35px;
    margin-left: 30px
}

.el-button-group {
    margin-top: 0;
    margin-left: 116px;
}

.class-table {
    justify-content: center;
    align-items: center;
    overflow-x: hidden;

    .table-wrapper {
        width: 600px;
        height: 500px;
        overflow: auto;
    }

    .tabel-container {

        table {
            table-layout: fixed;
            width: 100%;

            thead {
                background-color: #67a1ff;

                th {
                    color: #fff;
                    line-height: 10px;
                    font-weight: normal;
                }
            }

            tbody {
                background-color: #eaf2ff;

                td {
                    color: #677998;
                    line-height: 11px;
                }
            }

            th,
            td {
                width: 60px;
                padding: 12px 2px;
                font-size: 9px;
                text-align: center;
            }

            tr td:first-child {
                color: #333;

                .period {
                    font-size: 8px;
                }
            }
        }
    }
}</style>