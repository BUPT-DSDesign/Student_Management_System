<template>
    <div class='class-table'>
        <h3>课程表</h3>
        <div class='table-wrapper'>
            <div class='tabel-container'>
                <table>
                    <thead>
                        <tr>
                            <th>时间</th>
                            <th v-for='(week, index) in weeks' :key='index'> {{ '周' + digital2Chinese(index + 1, 'week') }}
                            </th>
                        </tr>
                    </thead>
                    <tbody>
                        <tr v-for='(item, index) in classTableData' :key='index'>
                            <td>
                                <p>{{ '第' + digital2Chinese(index + 1) + '节' }}</p>
                                <p class='period'>{{ item.classesTime }}</p>
                            </td>

                            <td v-for='(week, index) in weeks' :key='index'>
                                {{ item[week] || '-' }}
                            </td>
                        </tr>
                    </tbody>
                </table>
            </div>
        </div>
        <!-- <div class="search-button">
            <el-button type="primary" icon="el-icon-search">查询课程信息</el-button>
        </div> -->

        <el-button class="search-button" type="primary" icon="el-icon-search"
            @click="dialogFormVisible = true">查询课程信息</el-button>

        <el-dialog title="课程查询" :visible.sync="dialogFormVisible">
            <el-form :model="form">
                关键字
                <el-radio-group v-model="radio"  >
                    <el-radio :label="3">课程名称</el-radio>
                    <el-radio :label="6">课程时间</el-radio>
                    <el-radio :label="9">课程地点</el-radio>
                </el-radio-group>
            </el-form>
            <el-form :inline="true" :model="formInline" class="demo-form-inline">
                <el-form-item label="输入关键字">
                    <el-input v-model="formInline.user" placeholder="请输入关键字"></el-input>
                </el-form-item>
                <el-form-item>
                    <el-button type="primary" @click="onSubmit">查询</el-button>
                </el-form-item>
            </el-form>

            <div slot="footer" class="dialog-footer">
                <el-button @click="dialogFormVisible = false">取 消</el-button>
                <el-button type="primary" @click="dialogFormVisible = false">确 定</el-button>
            </div>
        </el-dialog>


        <div class="page-button">
            <el-button-group>
                <el-button type="primary" icon="el-icon-arrow-left">上一周</el-button>
                <el-button type="primary">下一周<i class="el-icon-arrow-right el-icon--right"></i></el-button>
            </el-button-group>
        </div>
    </div>
</template>
<script>

export default {
    data() {
        return {
            formInline: {
                user: '',
                region: ''
            },
            tableShow: false,
            dialogFormVisible: false,
            form: {
                name: '',
                region: '',
            },
            formLabelWidth: '120px',
            weeks: ['monday', 'tuesday', 'wednesday', 'thursday', 'friday', 'saturday', 'sunday'],
            classTableData: [{
                'classesTime': '08:00-09:00',
                'monday': '物理',
                'tuesday': '英语',
                'wednesday': '政治',
                'thursday': '历史',
                'friday': '化学',
                'saturday': '历史',
                'sunday': '化学'
            }, {
                'classesTime': '09:00-10:00',
                'monday': '生物',
                'tuesday': '物理',
                'wednesday': '化学',
                'thursday': '英语',
                'friday': '化学',
                'saturday': '生物',
                'sunday': '化学'
            }, {
                'classesTime': '10:00-11:00',
                'monday': '生物',
                'tuesday': '物理',
                'wednesday': '生物',
                'thursday': '历史',
                'friday': '生物',
                'saturday': '英语',
                'sunday': '政治'
            }, {
                'classesTime': '11:00-12:00',
                'monday': '',
                'tuesday': '政治',
                'wednesday': '物理',
                'thursday': '政治',
                'friday': '历史',
                'saturday': '历史',
                'sunday': '生物'
            }, {
                'classesTime': '13:00-14:00',
                'monday': '生物',
                'tuesday': '历史',
                'wednesday': '历史',
                'thursday': '历史',
                'friday': '',
                'saturday': '英语',
                'sunday': '化学'
            }, {
                'classesTime': '14:00-15:00',
                'monday': '化学',
                'tuesday': '英语',
                'wednesday': '物理',
                'thursday': '化学',
                'friday': '语文',
                'saturday': '物理',
                'sunday': '英语'
            }, {
                'classesTime': '15:00-16:00',
                'monday': '历史',
                'tuesday': '历史',
                'wednesday': '语文',
                'thursday': '历史',
                'friday': '生物',
                'saturday': '英语',
                'sunday': ''
            }],
        };
    },
    methods: {
        /**
        * 数字转中文
        * @param {Number} num 需要转换的数字
        * @param {String} identifier 标识符
        * @returns {String} 转换后的中文
        */
        digital2Chinese(num, identifier) {
            const character = ['零', '一', '二', '三', '四', '五', '六', '七', '八', '九', '十', '十一', '十二'];
            return identifier === 'week' && (num === 0 || num === 7) ? '日' : character[num];
        },
         onSubmit() {
            console.log('submit!');
        }
    }
};
</script>

<style lang='scss' scoped>
.page-button {
    float: right;
    margin-right: 150px;
}

.search-button {
    float: left;
    margin-left: 150px;
}

h3 {
    margin: 5px;
    font-size: 20px;
    text-align: center;
    color: black;
}

.class-table {
    .table-wrapper {
        margin: auto;
        width: 800px;
        height: 100%;
        overflow: auto;
    }

    .tabel-container {
        margin: 5px;

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
                font-size: 12px;
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
