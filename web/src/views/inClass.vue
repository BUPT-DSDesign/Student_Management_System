<template>
    <!-- 左侧：课表，切换功能，点击弹窗功能 -->
    <div class='class-table'>
        <div class='table-wrapper'>
            <div class='tabel-container'>
                <table>
                    <thead>
                        <tr>
                            <th>时间</th>
                            <th v-for='(week, index) in weeks'> {{ week }}</th>
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
        <el-tag effect="dark">第{{ curWeek }}周</el-tag>
        <el-button-group>
            <el-button type="primary" icon="el-icon-arrow-left" @click="toLastweek()">上一周</el-button>
            <el-button type="primary" @click="toNextweek()">下一周<i
                    class="el-icon-arrow-right el-icon--right"></i></el-button>
        </el-button-group>
        <!-- 弹窗 -->
        <el-dialog title="课程详情" :visible.sync="dialogVisible" width="30%" :before-close="handleClose">
            <el-card class="box-card">
                <div class="course_name">课程名称：{{ curClassData.course_name }}</div>
                <div class="course_name">课程时间：{{ curClassData.section_list }}</div>
                <div class="course_name">课程地点：{{ curClassData.classroom }}</div>
            </el-card>
            <span slot="footer" class="dialog-footer">
                <el-button @click="dialogVisible = false">取 消</el-button>
                <el-button type="primary" @click="dialogVisible = false">确 定</el-button>
            </span>
        </el-dialog>
        <!-- 右侧：课程搜索功能，通过选择课程名称，时间，地点，对关键字进行模糊匹配，查询后直接在下方显示 -->
        <el-row :gutter="12">
            <el-col :span="100">
                <el-card shadow="always">
                    <h2>关键字选择</h2>
                    <el-radio-group v-model="radio">
                        <el-radio :label="1">课程名称</el-radio>
                        <el-radio :label="2">课程地点</el-radio>
                    </el-radio-group>
                    <h2>查询课程</h2>

                    <el-form label-width="5px">
                        <el-form>
                            <el-input v-model="keyWord" placeholder="请输入查询关键词"></el-input>
                        </el-form>
                                <ul class="list-group">
                                    <li v-for="(p, index) of filclasslist" :key="index" @click="chooseclass($event)">
                                        {{ radio==1?p.course_name:p.classroom }}
                                    </li>
                                </ul>
                        <el-form-item>
                                <el-button type="primary" class="searchBtn" @click="onSubmit">查询</el-button>
                        </el-form-item>
                    </el-form>
                </el-card>
            </el-col>
        </el-row>

    </div>
</template>
<script>
export default {
    data() {
        return {
            curClass: '', //记录当前点击单元格课程的名称
            curClassData: {},  //记录当前点击单元格课程的信息（对象类型
            curWeek: 1,
            weeks: ['周一', '周二', '周三', '周四', '周五', '周六', '周日'],
            time: ['08:00-09:00', '09:00-10:00', '10:00-11:00', '11:00-12:00', '13:00-14:00', '14:00-15:00', '15:00-16:00', '16:00-17:00', '17:00-18:00'],
            classData: [{
                course_name: '计网',
                course_id: 1,
                week_schedule: [1, 2, 3, 4, 5],
                section_list: [1, 2, 3],
                classroom: "教三211",
            }, {
                course_name: '计算机组成原理',
                course_id: 1,
                week_schedule: [1, 2, 3, 4],
                section_list: [5, 6], //注意这里周的排列是一列一列来的
                classroom: "教三217",
            }],
            dialogVisible: false, //弹窗的可见性
            radio: 1,//多选框默认选中的单元
            keyWord: '', //用户查询的课程关键字
            filclasslist: [], //模糊匹配后的课程列表

        };
    },
    watch: {
        keyWord(newvalue) {
            //如果当前关键词为空
            if (newvalue === "") {
                this.filclasslist = [];
            }
            else {
                if (this.radio == 1) {
                    //有点问题，第二次过滤得到课程名时过滤不了
                    this.filclasslist = this.classData.filter((item) => {
                        return item.course_name.indexOf(newvalue) != -1;
                    });
                }
                else if (this.radio == 2) {
                    this.filclasslist = this.classData.filter((item) => {
                        return item.classroom.indexOf(newvalue) != -1;
                    });
                }
            }
        },
    },
    methods: {
        toNextweek() {
            if (this.curWeek >= 19) {
                return;
            }
            this.curWeek++;
            this.curCell = 0;//重置单元格计数器
            this.$forceUpdate(); //重新刷新页面
        },
        toLastweek() {
            if (this.curWeek <= 1) {
                return;
            }
            this.curWeek--;
            this.curCell = 0;//重置单元格计数器
            this.$forceUpdate(); //重新刷新页面
        },
        clickClass(event) {
            //更改当前点击课程的名称
            this.curClass = event.target.innerText;
            // 循环遍历找到数据库中该课程的所有信息
            for (let i = 0; i < 2; i++) {
                if (this.classData[i].course_name == this.curClass) {
<<<<<<< HEAD

=======
                     
>>>>>>> d69c02aaa8b3fe14d4f26bd9f2433deecade5e00
                    console.log('相等')
                    //如果有该课程，更新curClassData
                    this.curClassData = this.classData[i];
                }
            }
            //点击弹窗
            this.dialogVisible = true;
        },
        // 关闭弹窗
        handleClose(done) {
            done();
        },
        //查询点击事件
        onSubmit() {
            //提交后台进行查询
        },
        //点击ul自动填充input
        chooseclass(e) {
            this.keyWord = e.target.innerText;
        },
    },
    computed: {
        filltable() {
            return function (classIndex, weekIndex) {
                // 因为现在数据中只有两种课程，所以这里i < 2
                for (let i = 0; i < 2; i++) {
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
ul{
    margin-top:20px;
}
.searchBtn {
    margin-top: 20px;
}

h2 {
    margin: 18px;
    margin-left: 10px;
    font-size: 20px;
}

.el-card {
    float: right;
    margin-top: -500px;
    margin-left: 650px;
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
                    line-height: 17px;
                    font-weight: normal;
                }
            }

            tbody {
                background-color: #eaf2ff;

                td {
                    color: #677998;
                    line-height: 12px;
                }
            }

            th,
            td {
                width: 60px;
                padding: 12px 2px;
                font-size: 10px;
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
}
</style>