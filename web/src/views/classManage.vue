<template>
    <div style="margin:20px">
        <div style="text-align: center;"><h1>课程管理</h1></div>

        <el-table :data="classData" height="380px" border>
            <el-table-column type="index" width="10px" align="center"></el-table-column>
            <el-table-column prop="course_name" label="课程名称" width="140" align="center">
            </el-table-column>
            <el-table-column prop="teacher" label="上课老师" width="120" align="center">
            </el-table-column>
            <el-table-column prop="teacher" label="上课地点" width="120" align="center">
            </el-table-column>
            <el-table-column prop="classTime" label="课程时间" width="260" align="center">
            </el-table-column>
            <el-table-column prop="exam_option" label="考核方式" width="100" align="center">

            </el-table-column>
            <el-table-column align="right">
                <template slot="header" slot-scope="scope">
                    <el-input v-model="search" size="mini" placeholder="输入关键字搜索" prefix-icon="el-icon-search" />
                </template>
                <template slot-scope="scope">
                    <el-button size="mini" type="success" @click="seeCourseInfo(scope.row)">查看详情</el-button>
                    <el-button size="mini" type="primary" @click="editClass(scope.$index, scope.row)">修改课程</el-button>
                    <el-button size="mini" type="danger" @click="deleteClass(scope.$index, scope.row)">删除课程</el-button>
                </template>
            </el-table-column>
        </el-table>
        <el-button style="float: right; margin-top: 10px" type="primary" @click="addClass">添加课程</el-button>
        <!-- 修改课程 -->
        <el-dialog title="修改课程" :visible.sync="dialogVisible2" width="600px">
            <el-form :model="clickedClassData" label-width="170px" style="backgroundColor:#fff">
                <el-form-item label="课程名称">
                    <el-input v-model="clickedClassData.course_name"></el-input>
                </el-form-item>
                <el-form-item label="上课地点">
                    <el-input v-model="clickedClassData.classroom"></el-input>
                </el-form-item>
                <el-form-item label="授课老师">
                    <el-input v-model="clickedClassData.teacher"></el-input>
                </el-form-item>
                <el-form-item label="联系方式">
                    <el-input v-model="clickedClassData.contact"></el-input>
                </el-form-item>
                <el-form-item label="上课节次">
                    <el-cascader v-model="clickedClassData.section_list" :options="options" :props="{ multiple: true }"
                        filterable ></el-cascader>
                </el-form-item>
                <el-form-item label="上课周次">
                    <el-select v-model="clickedClassData.week_schedule" placeholder="请选择" multiple>
                        <el-option v-for="item in week_options" :key="item.value" :label="item.label" :value="item.value">
                        </el-option>
                    </el-select>
                </el-form-item>
                <el-form-item label="考试时间">
                    <el-input v-model="clickedClassData.exam_time"></el-input>
                </el-form-item>
                <el-form-item label="考试地点">
                    <el-input v-model="clickedClassData.exam_location"></el-input>
                </el-form-item>
                <el-form-item label="考核方式">
                    <el-radio-group v-model="clickedClassData.exam_option">
                        <el-radio :label="0">论文考察</el-radio>
                        <el-radio :label="1">线下考试</el-radio>
                        <el-radio :label="2">线上考试</el-radio>
                    </el-radio-group>
                </el-form-item>

                <el-form-item label="是否为线上课程">
                    <el-radio-group v-model="clickedClassData.is_course_online">
                        <el-radio :label="1">是</el-radio>
                        <el-radio :label="0">否</el-radio>
                    </el-radio-group>
                </el-form-item>
                <el-form-item label="是否为必修课程">
                    <el-radio-group v-model="clickedClassData.is_compulsory">
                        <el-radio :label="1">是</el-radio>
                        <el-radio :label="0">否</el-radio>
                    </el-radio-group>
                </el-form-item>
            </el-form>
            <div slot="footer" class="dialog-footer">
                <el-button @click="dialogVisible4 = false">取 消</el-button>
                <el-button type="primary" @click="submitEditForm">保存修改</el-button>
            </div>
        </el-dialog>      
        
        <!-- 添加课程 -->
        <el-dialog title="添加课程" :visible.sync="dialogVisible4" width="600px">
            <el-form :model="addClassData" label-width="170px" style="backgroundColor:#fff">
                <el-form-item label="课程名称">
                    <el-input v-model="addClassData.course_name"></el-input>
                </el-form-item>
                <el-form-item label="上课地点">
                    <el-input v-model="addClassData.classroom"></el-input>
                </el-form-item>
                <el-form-item label="授课老师">
                    <el-input v-model="addClassData.teacher"></el-input>
                </el-form-item>
                <el-form-item label="联系方式">
                    <el-input v-model="addClassData.contact"></el-input>
                </el-form-item>
                <el-form-item label="上课节次">
                    <el-cascader v-model="addClassData.section_list" :options="options" :props="{ multiple: true }"
                        filterable ></el-cascader>
                </el-form-item>
                <el-form-item label="上课周次">
                    <el-select v-model="addClassData.week_schedule" placeholder="请选择" multiple>
                        <el-option v-for="item in week_options" :key="item.value" :label="item.label" :value="item.value">
                        </el-option>
                    </el-select>
                </el-form-item>
                <el-form-item label="考试时间">
                    <el-input v-model="addClassData.exam_time"></el-input>
                </el-form-item>
                <el-form-item label="考试地点">
                    <el-input v-model="addClassData.exam_location"></el-input>
                </el-form-item>
                <el-form-item label="考核方式">
                    <el-radio-group v-model="addClassData.exam_option">
                        <el-radio :label="0">论文考察</el-radio>
                        <el-radio :label="1">线下考试</el-radio>
                        <el-radio :label="2">线上考试</el-radio>
                    </el-radio-group>
                </el-form-item>

                <el-form-item label="是否为线上课程">
                    <el-radio-group v-model="addClassData.is_course_online">
                        <el-radio :label="1">是</el-radio>
                        <el-radio :label="0">否</el-radio>
                    </el-radio-group>
                </el-form-item>
                <el-form-item label="是否为必修课程">
                    <el-radio-group v-model="addClassData.is_compulsory">
                        <el-radio :label="1">是</el-radio>
                        <el-radio :label="0">否</el-radio>
                    </el-radio-group>
                </el-form-item>
            </el-form>
            <div slot="footer" class="dialog-footer">
                <el-button @click="dialogVisible4 = false">取 消</el-button>
                <el-button type="primary" @click="submitAddForm">添 加</el-button>
            </div>
        </el-dialog>
        <!-- 查看课程详情 -->
        <el-dialog :visible.sync="seeCourseInfoVis" title="课程详细信息" :style="{ 'max-height': '80%' }">
            <span>{{ courseInfo.course_name }}</span>
          <div style="padding: 20px">
            <p><strong>上课地点：</strong>{{ courseInfo.classroom }}</p>
            <p><strong>授课老师：</strong>{{ courseInfo.teacher }}</p>
            <p><strong>上课时间：</strong>{{ courseInfo.class_time }}</p>
            <p><strong>上课周次：</strong>{{ courseInfo.week_schedule }}</p>
            <p><strong>考试时间：</strong>{{ courseInfo.exam_time }}</p>
            <p><strong>考试地点：</strong>{{ courseInfo.exam_location }}</p>
          </div>
      </el-dialog>
    </div>
</template>

<script>
import { CourseStore } from '@/store/course';
import { LogStore } from '@/store/log';
import { TimeStore } from '@/store/time';

const startTimeMap = {
    1: "8:00",
    2: "9:00",
    3: "10:00",
    4: "11:00",
    5: "13:00",
    6: "14:00",
    7: "15:00",
    8: "16:00",
    9: "17:00",
};
const endTimeMap = {
    1: "9:00",
    2: "10:00",
    3: "11:00",
    4: "12:00",
    5: "14:00",
    6: "15:00",
    7: "16:00",
    8: "17:00",
    9: "18:00",
}
const sectionMap = [{
    value: 1,
    label: '第一节'
}, {
    value: 2,
    label: '第二节'
}, {
    value: 3,
    label: '第三节'
}, {
    value: 4,
    label: '第四节'
}, {
    value: 5,
    label: '第五节'
}, {
    value: 6,
    label: '第六节'
}, {
    value: 7,
    label: '第七节'
}, {
    value: 8,
    label: '第八节'
}, {
    value: 9,
    label: '第九节'
},
];
export default {
    created() {
        const getTable = async () => {
            const fg = await CourseStore.GetAllCourse();
            if (fg) {
                this.classData = CourseStore.allCourseList
                console.log(this.classData)
                //添加一个属性以便格式化课程时间
                for (let i = 0; i < this.classData.length; i++) {
                    let start = 0;
                    let end = 0;
                    for (let j = 0; j < this.classData[i].section_list.length - 1; j++) {
                        if (this.classData[i].section_list[j] + 1 == this.classData[i].section_list[j + 1]) {
                            end++;
                        }
                        else {
                            let startTime = startTimeMap[this.classData[i].section_list[start] % 9];
                            let endTime = endTimeMap[this.classData[i].section_list[end] % 9];
                            if (this.classData[i].hasOwnProperty('classTime')) {
                                this.classData[i]['classTime'] = this.classData[i]['classTime'] + ',' + startTime + '-' + endTime;
                            }
                            else {
                                this.classData[i]['classTime'] = startTime + '-' + endTime;
                            }
                            start = end + 1;
                            end = start;
                        }

                    }
                    let startTime = startTimeMap[this.classData[i].section_list[start] % 9];
                    let endTime = endTimeMap[this.classData[i].section_list[end] % 9];
                    if (this.classData[i].hasOwnProperty('classTime')) {
                        this.classData[i]['classTime'] = this.classData[i]['classTime'] + ',' + startTime + '-' + endTime;
                    }
                    else {
                        this.classData[i]['classTime'] = startTime + '-' + endTime;
                    }

                    // 根据this.classData[i].exam_option的值来判断考核方式
                    if (this.classData[i].exam_option == 0) {
                        this.classData[i].exam_option = '论文考察'
                    } else if (this.classData[i].exam_option == 1) {
                        this.classData[i].exam_option = '线下考试'
                    } else if (this.classData[i].exam_option == 2) {
                        this.classData[i].exam_option = '线上考试'
                    }

                }
            } else {
                console.log('error')
            }
        }
        getTable()

    },
    data() {
        return {
            classData: [],
            tableData: [],
            clickedClass: '',  //当前点击的单元格的课程名称
            clickedClassData: {},  //记录当前点击单元格课程的信息（对象类型
            addClassData: {},//添加的课程的信息
            dialogVisible1: false, //点击课程弹窗的可见性
            dialogVisible2: false, //添加课程弹窗的可见性
            dialogVisible3: false, //发布考试弹窗的可见性
            dialogVisible4: false,//添加课程弹窗的可见性
            search: '', //用于搜索过滤的对象
            radio3: 1,
            week_num: [],
            week_options: [{
                value: 1,
                label: '第一周'
            }, {
                value: 2,
                label: '第二周'
            }, {
                value: 3,
                label: '第三周'
            }, {
                value: 4,
                label: '第四周'
            }, {
                value: 5,
                label: '第五周'
            }, {
                value: 6,
                label: '第六周'
            }, {
                value: 7,
                label: '第七周'
            }, {
                value: 8,
                label: '第八周'
            }, {
                value: 9,
                label: '第九周'
            }, {
                value: 10,
                label: '第十周'
            }, {
                value: 11,
                label: '第十一周'
            }, {
                value: 12,
                label: '第十二周'
            }, {
                value: 13,
                label: '第十三周'
            }, {
                value: 14,
                label: '第十四周'
            }, {
                value: 15,
                label: '第十五周'
            }
            ],
            options: [
                {
                    value: 1,
                    label: '周一',
                    children: sectionMap,
                },
                {
                    value: 2,
                    label: '周二',
                    children: sectionMap,
                },
                {
                    value: 3,
                    label: '周三',
                    children: sectionMap,
                },
                {
                    value: 4,
                    label: '周四',
                    children: sectionMap,
                },
                {
                    value: 5,
                    label: '周五',
                    children: sectionMap,
                },
                {
                    value: 6,
                    label: '周六',
                    children: sectionMap,
                },
                {
                    value: 7,
                    label: '周天',
                    children: sectionMap,
                },
            ],
            selectedCourse: {},
            courseInfo: {},
            seeCourseInfoVis: false
        }
    },
    methods: {
        getClassTime(sectionList) {
            const days = ['周一', '周二', '周三', '周四', '周五', '周六', '周日'];
            const sections = ['第一节', '第二节', '第三节', '第四节', '第五节', '第六节', '第七节', '第八节', '第九节'];
            const classTime = sectionList.map(section => {
                const day = Math.floor((section - 1) / 9); // 获取星期几
                const sectionIndex = (section - 1) % 9; // 获取第几节课
                return days[day] + sections[sectionIndex];
            });
            return classTime.join('，');
        },
        getWeekSchedule(weekSchedule) {
            const weeks = [];
            for (let i = 0; i < weekSchedule.length; i++) {
                if (weekSchedule[i] === 1) {
                    weeks.push(i + 1);
                }
            }
            return weeks.join('，') + '周';
        },
        seeCourseInfo(course) {
            this.selectedCourse = course;
            this.courseInfo = this.processCourseInfo(course);
            this.seeCourseInfoVis = true;
            
        },
         processCourseInfo(course) {
            const days = ['周一', '周二', '周三', '周四', '周五', '周六', '周日'];
            const sections = ['第一节', '第二节', '第三节', '第四节', '第五节', '第六节', '第七节', '第八节', '第九节'];
            const classTime = course.section_list.map(section => {
                const day = Math.floor((section - 1) / 9); // 获取星期几
                const sectionIndex = (section - 1) % 9; // 获取第几节课
                return days[day] + sections[sectionIndex];
            }).join('，');
            const weekSchedule = course.week_schedule.map((week, index) => week === 1 ? index + 1 : null).filter(week => week !== null).join('，') + '周';
            return {
                course_name: course.course_name,
                classroom: course.classroom,
                teacher: course.teacher,
                class_time: classTime,
                week_schedule: weekSchedule,
                exam_time: course.exam_time,
                exam_location: course.exam_location
            };
        },
        addClass() {
            this.dialogVisible4 = true;
        },
        clickcell(e) {
            this.clickedClass = e.classname;
            // 循环遍历找到数据库中该课程的所有信息
            for (let i = 0; i < this.classData.length; i++) {
                if (this.classData[i].course_name == this.clickedClass) {
                    //如果有该课程，更新curClassData
                    this.clickedClassData = this.classData[i];
                    console.log(this.clickedClassData)
                }
            }
            //点击弹窗
            this.dialogVisible1 = true;
        },
        // 关闭弹窗
        handleClose(done) {
            done();
        },
        //编辑课程
        editClass(index, row) {
            this.clickedClassData = row;
            //点击弹窗打开
            this.dialogVisible2 = true;
        },
        //删除课程
        deleteClass(index, row) {
        
            const deleteCourse = async (data) => {
                const fg = await CourseStore.DeleteCourseInfo(data);
                if (fg) {
                    this.$message({
                        showClose: true,
                        center: true,
                        message: '删除课程成功',
                        type: 'success'
                    });
                    // 将添加课程的信息添加到日志中
                    const log = {
                        "create_time": TimeStore.getTime(),
                        "content": "删除课程成功, 课程名：" + name
                    }
                    console.log(log)
                    LogStore.AddLog(log)
                    this.deleteClassData = {}
                } else {
                    this.$message({
                        showClose: true,
                        center: true,
                        message: '删除课程失败',
                        type: 'error'
                    });
                    this.deleteClassData = {}
                }
            }

            deleteCourse(row.course_id);
        },
       //修改课程
        submitEditForm() {
            this.dialogVisible2 = false;
            let week_schedule = [];
            this.clickedClassData.week_schedule.forEach(function (item) {
                week_schedule.push(item);
            })
            week_schedule.sort();
            this.clickedClassData.week_schedule = week_schedule;

            let section_list = [];
            this.clickedClassData.section_list.forEach(function (item) {
                section_list.push((item[0] - 1) * 9 + item[1]);
            })
            section_list.sort();
            this.clickedClassData.section_list = section_list;

            this.clickedClassData.is_course_online = '1' ? true : false;
            this.clickedClassData.is_compulsory = '1' ? true : false;

            const editCourse = async (data) => {
                const fg = await CourseStore.EditCourseInfo(data);
                if (fg) {
                    this.$message({
                        showClose: true,
                        center: true,
                        message: '修改课程成功',
                        type: 'success'
                    });
                    // 将添加课程的信息添加到日志中
                    const log = {
                        "create_time": TimeStore.getTime(),
                        "content": "修改课程信息成功, 课程名：" + data.course_name
                    }
                    console.log(log)
                    LogStore.AddLog(log)
                    this.clickedClassData = {}
                } else {
                    this.$message({
                        showClose: true,
                        center: true,
                        message: '修改课程失败',
                        type: 'error'
                    });
                    this.clickedClassData = {}
                }
            }
            editCourse(this.clickedClassData)
        },
        //添加课程
        submitAddForm() {
            this.dialogVisible4 = false;
            let week_schedule = [];
            this.addClassData.week_schedule.forEach(function (item) {
                week_schedule.push(item);
            })
            week_schedule.sort();
            this.addClassData.week_schedule = week_schedule;

            let section_list = [];
            this.addClassData.section_list.forEach(function (item) {
                section_list.push((item[0] - 1) * 9 + item[1]);
            })
            section_list.sort();
            this.addClassData.section_list = section_list;
            this.addClassData.is_course_online = this.addClassData.is_course_online == 1 ? true : false;
            this.addClassData.is_compulsory = this.addClassData.is_compulsory == 1 ? true : false;

            const addCourse = async (data) => {
                console.log(data)
                const fg = await CourseStore.AddCourseInfo(data);
                if (fg) {
                    this.$message({
                        showClose: true,
                        center: true,
                        message: '添加课程成功',
                        type: 'success'
                    });
                    // 将添加课程的信息添加到日志中
                    const log = {
                        "create_time": TimeStore.getTime(),
                        "content": "添加课程---" + this.addClassData.course_name
                    }
                    console.log(log)
                    LogStore.AddLog(log)
                    location.reload();
                    this.addClassData = {}
                                       
                } else {
                    this.$message({
                        showClose: true,
                        center: true,
                        message: '添加课程失败',
                        type: 'error'
                    });
                    this.addClassData = {}
                }
            }
            addCourse(this.addClassData) 
        },
    },
}
</script>
<style></style>