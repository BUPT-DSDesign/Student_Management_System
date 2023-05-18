import { defineStore } from 'pinia'
<<<<<<< HEAD
import { CourseTable, AddCourse, DeleteCourse, EditCourse, inquiryCourse } from '@/api/course'
=======
import { CourseTable, AddCourse, SelectiveCourse, AllCourse, CourseSelect } from '@/api/course'
>>>>>>> 90f1f0686213ab03738f971f3282d654557bb74c
import { ref } from 'vue'

export const useCourseStore = defineStore('course', () => {
    const courseList = ref([])
<<<<<<< HEAD
    const searchCourseList = ref([])


=======
    const selectiveCourseList = ref([])
>>>>>>> 90f1f0686213ab03738f971f3282d654557bb74c
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
<<<<<<< HEAD
    const DeleteCourseInfo = async (data) => {
        try {
            const res = await DeleteCourse(data)
=======

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
>>>>>>> 90f1f0686213ab03738f971f3282d654557bb74c
            console.log(res.data)
            if (res.data.status_code == 0) {
                return true
            }
        } catch (err) {
            return false
        }
    }
<<<<<<< HEAD
    const EditCourseInfo = async (data) => {
        try {
            const res = await EditCourse(data)
            console.log(res.data)
            if (res.data.status_code == 0) {
                return true
            }
        } catch (err) {
            return false
        }
    }
    const inquiryCourseInfo = async (data) => {
        try {
            const res = await inquiryCourse(data)
            console.log(res.data)
            if (res.data.status_code == 0) {
                searchCourseList.value = res.data.course_list
=======

    const SelectCourse = async (data) => {
        try {
            const res = await CourseSelect(data)
            console.log(res.data)
            if (res.data.status_code == 0) {
                GetSelectiveCourse()
>>>>>>> 90f1f0686213ab03738f971f3282d654557bb74c
                return true
            }
        } catch (err) {
            return false
        }
    }

    return {
        courseList,
        searchCourseList,
        GetCourseTable,
        AddCourseInfo,
<<<<<<< HEAD
        DeleteCourseInfo,
        EditCourseInfo,
        inquiryCourseInfo
=======
        GetSelectiveCourse,
        GetAllCourse,
        selectiveCourseList,
        SelectCourse
>>>>>>> 90f1f0686213ab03738f971f3282d654557bb74c
    }
})
