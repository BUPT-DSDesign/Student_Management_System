<template>
    <div style="margin:20px">
        <h1>课程管理</h1>
        <el-table :data="tableData" height="380px" border style="width: 800px">
            <el-table-column prop="classname" label="课程名称" width="180">
            </el-table-column>
            <el-table-column prop="classtime" label="课程时间" width="280">
            </el-table-column>
            <el-table-column align="right">
                <template slot="header" slot-scope="scope">
                    <el-input v-model="search" size="mini" placeholder="输入关键字搜索" prefix-icon="el-icon-search" />
                </template>
                <template slot-scope="scope">
                    <el-button size="mini" type="primary" @click="editClass(scope.$index, scope.row)">修改课程</el-button>
                    <el-button size="mini" type="success" @click="publishTest(scope.$index, scope.row)">发布考试</el-button>
                    <el-button size="mini" type="danger" @click="deleteClass(scope.$index, scope.row)">删除课程</el-button>
                </template>
            </el-table-column>
        </el-table>
        <el-button type="primary" plain @click="addclass">添加课程</el-button>

        <!-- 点击弹窗 -->
        <el-dialog title="课程详情" :visible.sync="dialogVisible1" width="30%" :before-close="handleClose">
            <div class="course_name">课程名称：{{ clickedClassData.course_name }}</div>
            <div class="course_time">课程时间：{{ clickedClassData.classTime }}</div>
            <div class="course_address">课程地点：{{ clickedClassData.classroom }}</div>
            <span slot="footer" class="dialog-footer">
                <el-button @click="dialogVisible1 = false">取 消</el-button>
                <el-button type="primary" @click="dialogVisible1 = false">确 定</el-button>
            </span>
        </el-dialog>
        <!-- 修改课程 -->
        <el-dialog title="课程详情" :visible.sync="dialogVisible2" width="500px" :before-close="handleClose">
            <el-form ref="clickedClassData" :model="clickedClassData" label-width="150px" style="backgroundColor:#fff">
                <el-form-item label="课程名称">
                    <el-input v-model="clickedClassData.course_name"></el-input>
                </el-form-item>
                <el-form-item label="授课老师">
                    <el-input v-model="clickedClassData.teacher"></el-input>
                </el-form-item>
                <el-form-item label="上课节次">
                    <el-input v-model="clickedClassData.section_list"></el-input>
                </el-form-item>
                <el-form-item label="上课周次">
                    <el-input v-model="clickedClassData.week_schedule"></el-input>
                </el-form-item>
                <el-form-item label="考试时间">
                    <el-input v-model="clickedClassData.exam_time"></el-input>
                </el-form-item>
                <el-form-item label="考试地点">
                    <el-input v-model="clickedClassData.exam_location"></el-input>
                </el-form-item>
                <el-form-item>
                    <el-button type="primary" @click="submitEdit">保存修改</el-button>
                    <el-button @click="dialogVisible2 = false">取消</el-button>
                </el-form-item>
            </el-form>
        </el-dialog>
        <!-- 发布考试 -->
        <el-dialog title="发布考试" :visible.sync="dialogVisible3" width="500px" :before-close="handleClose">
            <el-form ref="clickedClassData" :model="clickedClassData" label-width="150px" style="backgroundColor:#fff">
                <el-form-item label="考试时间">
                    <el-input v-model="clickedClassData.exam_time"></el-input>
                </el-form-item>
                <el-form-item label="考试地点">
                    <el-input v-model="clickedClassData.exam_location"></el-input>
                </el-form-item>
                <el-form-item>
                    <el-button type="primary" @click="submitTest">发布</el-button>
                    <el-button @click="dialogVisible3 = false">取消</el-button>
                </el-form-item>
            </el-form>
        </el-dialog>
        <!-- 添加课程 -->
        <el-dialog title="添加课程" :visible.sync="dialogVisible4" width="600px">
            <el-form :model="addClassData" label-width="170px" style="backgroundColor:#fff">
                <el-form-item label="课程名称">
                    <el-input v-model="addClassData.course_name"></el-input>
                </el-form-item>
                <el-form-item label="上课地点">
                    <el-input v-model="addClassData.section_list"></el-input>
                </el-form-item>
                <el-form-item label="授课老师">
                    <el-input v-model="addClassData.teacher"></el-input>
                </el-form-item>
                <el-form-item label="联系方式">
                    <el-input v-model="addClassData.contact"></el-input>
                </el-form-item>
                <el-form-item label="上课节次">
                    <el-cascader v-model="addClassData.section_list" :options="options" :props="{ multiple: true }"
                        filterable @change="section_list_change"></el-cascader>
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

    </div>
</template>

<script>
import { CourseStore } from '@/store/course';

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
            const fg = await CourseStore.GetCourseTable();
            if (fg) {
                this.classData = CourseStore.courseList
                //添加一个属性以便格式化课程时间
                for (let i = 0; i < this.classData.length; i++) {
                    let start = 0;
                    let end = 0;
                    for (let j = 0; j < this.classData[i].section_list.length - 1; j++) {
                        console.log(this.classData[i].section_list[j]);
                        console.log(this.classData[i].section_list[j + 1])
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
                    console.log("遍历完了")
                    let startTime = startTimeMap[this.classData[i].section_list[start] % 9];
                    let endTime = endTimeMap[this.classData[i].section_list[end] % 9];
                    if (this.classData[i].hasOwnProperty('classTime')) {
                        this.classData[i]['classTime'] = this.classData[i]['classTime'] + ',' + startTime + '-' + endTime;
                    }
                    else {
                        this.classData[i]['classTime'] = startTime + '-' + endTime;
                    }

                }
                for (let i = 0; i < this.classData.length; i++) {
                    this.tableData.push({
                        classname: this.classData[i].course_name,
                        classtime: this.classData[i].classTime,
                    })
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
            ]
        }
    },
    methods: {
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
        editClass(index, obj) {
            this.clickedClass = obj.classname;
            // 循环遍历找到数据库中该课程的所有信息
            for (let i = 0; i < this.classData.length; i++) {
                if (this.classData[i].course_name == this.clickedClass) {
                    //如果有该课程，更新curClassData
                    this.clickedClassData = this.classData[i];
                }
            }
            //点击弹窗打开
            this.dialogVisible2 = true;
        },
        //发布考试
        publishTest(index, obj) {
            this.clickedClass = obj.classname;
            // 循环遍历找到数据库中该课程的所有信息
            for (let i = 0; i < this.classData.length; i++) {
                if (this.classData[i].course_name == this.clickedClass) {
                    //如果有该课程，更新curClassData
                    this.clickedClassData = this.classData[i];
                }
            }
            //点击弹窗打开
            this.dialogVisible3 = true;
        },
        submitAddForm() {
            this.dialogVisible4 = false;
            console.log(this.addClassData);
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
            console.log(section_list)

            this.addClassData.section_list = section_list;

            this.addClassData.is_course_online = 1 ? true : false;
            this.addClassData.is_compulsory = 1 ? true : false;


            const addCourse = async (data) => {
                const fg = await CourseStore.AddCourseInfo(data);
                if (fg) {
                    this.$message({
                        showClose: true,
                        center: true,
                        message: '添加课程成功',
                        type: 'success'
                    });
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

            // location.reload();
        },
        addclass() {
            this.dialogVisible4 = true;

        }
    },
}
</script>
<style></style>