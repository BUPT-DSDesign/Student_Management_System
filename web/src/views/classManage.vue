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
        <el-dialog title="添加课程" :visible.sync="dialogVisible4" width="500px">
            <el-form :model="addClassData" label-width="150px" style="backgroundColor:#fff">
                <el-form-item label="课程名称">
                    <el-input v-model="addClassData.course_name"></el-input>
                </el-form-item>
                <el-form-item label="授课老师">
                    <el-input v-model="addClassData.teacher"></el-input>
                </el-form-item>
                <el-form-item label="上课节次">
                    <el-input v-model="addClassData.section_list"></el-input>
                </el-form-item>
                <el-form-item label="上课周次">
                    <el-input v-model="addClassData.week_schedule"></el-input>
                </el-form-item>
                <el-form-item label="考试时间">
                    <el-input v-model="addClassData.exam_time"></el-input>
                </el-form-item>
                <el-form-item label="考试地点">
                    <el-input v-model="addClassData.exam_location"></el-input>
                </el-form-item>
            </el-form>
            <div slot="footer" class="dialog-footer">
                <el-button @click="dialogVisible4 = false">取 消</el-button>
                <el-button type="primary" @click="dialogVisible4 = false">添 加</el-button>
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
                        console.log(this.classData[i].section_list[j+1])
                        if (this.classData[i].section_list[j]+1 == this.classData[i].section_list[j + 1]) {
                            console.log("连续的")
                            end++;
                        }
                        else {
                            console.log("不连续")
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
        addclass() {
            //点击弹窗打开
            this.dialogVisible4 = true;
        }
    },
}
</script>