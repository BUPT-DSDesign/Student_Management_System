import { defineStore } from 'pinia'
import { CourseTable, AddCourse, SelectiveCourse, AllCourse, CourseSelect } from '@/api/course'
import { ref } from 'vue'

export const useCourseStore = defineStore('course', () => {
    const courseList = ref([])
    const selectiveCourseList = ref([])
    const GetCourseTable = async () => {
        try {
            const res = await CourseTable()

            if (res.data.status_code == 0) {
                courseList.value = res.data.course_list
                return true
            }
            return false
        } catch (err) {
            return false
        }
    }

    const AddCourseInfo = async (data) => {
        try {
            const res = await AddCourse(data)
            console.log(res.data)
            if (res.data.status_code == 0) {
                return true
            }
        } catch (err) {
            return false
        }
    }

    const GetSelectiveCourse = async () => {
        try {
            const res = await SelectiveCourse()
            console.log(res.data)
            if (res.data.status_code == 0) {
                selectiveCourseList.value = res.data.course_list
                return true
            }
        } catch (err) {
            return false
        }
    }

    const GetAllCourse = async () => {
        try {
            const res = await AllCourse()
            console.log(res.data)
            if (res.data.status_code == 0) {
                return true
            }
        } catch (err) {
            return false
        }
    }

    const SelectCourse = async (data) => {
        try {
            const res = await CourseSelect(data)
            console.log(res.data)
            if (res.data.status_code == 0) {
                GetSelectiveCourse()
                return true
            }
        } catch (err) {
            return false
        }
    }

    return {
        courseList,
        GetCourseTable,
        AddCourseInfo,
        GetSelectiveCourse,
        GetAllCourse,
        selectiveCourseList,
        SelectCourse
    }
})
